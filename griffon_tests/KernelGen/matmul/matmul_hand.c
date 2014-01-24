//===----------------------------------------------------------------------===//
//
//     KernelGen -- A prototype of LLVM-based auto-parallelizing Fortran/C
//        compiler for NVIDIA GPUs, targeting numerical modeling code.
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <CL/cl.h>

void _GfnCheckCLStatus(cl_int status, const char *phase_name)
{
	if (status != CL_SUCCESS)
	{
		fprintf(stderr, "**[OpenCL] Runtime error in %s phase**\n", phase_name);

		switch(status) {
		// program is not a valid program object
		case CL_INVALID_PROGRAM:
		    fprintf(stderr, "Error : CL_INVALID_PROGRAM\n"); break;
		// there is a failure to build the program executable
		case CL_INVALID_ARG_INDEX:
			fprintf(stderr, "Error : CL_INVALID_ARG_INDEX\n"); break;
		case CL_INVALID_ARG_VALUE:
			fprintf(stderr, "Error : CL_INVALID_ARG_VALUE\n"); break;
		case CL_INVALID_SAMPLER:
			fprintf(stderr, "Error : CL_INVALID_SAMPLER\n"); break;
		case CL_INVALID_ARG_SIZE:
			fprintf(stderr, "Error : CL_INVALID_ARG_SIZE\n"); break;
		case CL_BUILD_PROGRAM_FAILURE:
		    fprintf(stderr, "Error : CL_BUILD_PROGRAM_FAILURE\n"); break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			fprintf(stderr, "Error : CL_INVALID_PROGRAM_EXECUTABLE\n"); break;
		case CL_INVALID_BINARY:
		    fprintf(stderr, "Error : CL_INVALID_BINARY\n"); break;
		case CL_INVALID_BUILD_OPTIONS:
		    fprintf(stderr, "Error : CL_INVALID_BUILD_OPTIONS\n"); break;
		case CL_INVALID_COMMAND_QUEUE:
			fprintf(stderr, "Error : CL_INVALID_COMMAND_QUEUE\n"); break;
		case CL_INVALID_KERNEL:
			fprintf(stderr, "Error : CL_INVALID_KERNEL\n"); break;
		case CL_INVALID_KERNEL_NAME:
		    fprintf(stderr, "Error : CL_INVALID_KERNEL_NAME\n"); break;
		case CL_INVALID_KERNEL_DEFINITION:
		    fprintf(stderr, "Error : CL_INVALID_KERNEL_DEFINITION\n"); break;
		case CL_INVALID_CONTEXT:
			fprintf(stderr, "Error : CL_INVALID_CONTEXT\n"); break;
		case CL_INVALID_KERNEL_ARGS:
			fprintf(stderr, "Error : CL_INVALID_KERNEL_ARGS\n"); break;
		case CL_INVALID_WORK_DIMENSION:
			fprintf(stderr, "Error : CL_INVALID_WORK_DIMENSION\n"); break;
		case CL_INVALID_WORK_GROUP_SIZE:
			fprintf(stderr, "Error : CL_INVALID_WORK_GROUP_SIZE\n"); break;
		case CL_INVALID_WORK_ITEM_SIZE:
			fprintf(stderr, "Error : CL_INVALID_WORK_ITEM_SIZE\n"); break;
		case CL_INVALID_GLOBAL_OFFSET:
			fprintf(stderr, "Error : CL_INVALID_GLOBAL_OFFSET\n"); break;
		case CL_INVALID_MEM_OBJECT:
			fprintf(stderr, "Error : CL_INVALID_MEM_OBJECT\n"); break;
		case CL_INVALID_VALUE:
			fprintf(stderr, "Error : CL_INVALID_VALUE\n"); break;
		case CL_INVALID_DEVICE:
		    fprintf(stderr, "Error : CL_INVALID_DEVICE\n"); break;
		case CL_INVALID_OPERATION:
		    fprintf(stderr, "Error : CL_INVALID_OPERATION\n"); break;
		case CL_OUT_OF_RESOURCES:
			fprintf(stderr, "Error : CL_OUT_OF_RESOURCES\n"); break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			fprintf(stderr, "Error : CL_MEM_OBJECT_ALLOCATION_FAILURE\n"); break;
		case CL_INVALID_EVENT_WAIT_LIST:
			fprintf(stderr, "Error : CL_INVALID_EVENT_WAIT_LIST\n"); break;
		case CL_OUT_OF_HOST_MEMORY:
			fprintf(stderr, "Error : CL_OUT_OF_HOST_MEMORY\n"); break;
		case CL_COMPILER_NOT_AVAILABLE:
		    fprintf(stderr, "Error : CL_COMPILER_NOT_AVAILABLE\n"); break;
		default:
		    fprintf(stderr, "Error : CL unknown error value = %d\n", status); break;
		}
	}
}

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

int round_to(int x, int r) {
	int q = ceil(x/(float)r);
	return q * r;
}

#define parse_arg(name, arg) \
	int name = atoi(arg); \
	if (name < 0) \
	{ \
		printf("Value for " #name " is invalid: %d\n", name); \
		exit(1); \
	}

#define double_rand() (((double)(rand() / (double)RAND_MAX) - 0.5) * 2)

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void gpu_matmul(__global double *A,							\n"
"                         __global double *B,							\n"
"                         __global double *C,							\n"
"                         int nx,										\n"
"                         int ny,										\n"
"                         int nz,										\n"
"                         int start,									\n"
"                         int end)										\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j, k;														\n"
"																		\n"
"	if (tid < end) {													\n"
"		i = tid / nz;													\n"
"		j = tid % nz;													\n"
"		double tmp = 0.0;												\n"
"		for (k = 0; k < ny; ++k) {										\n"
"			tmp += A[i*ny+k] * B[k*nz+j];								\n"
"		}																\n"
"		C[i*nz+j] = tmp;												\n"
"	}																	\n"
"}																		\n"
;

void matmul_(int nx, int ny, int nz, double* A, double* B, double* C,
			 int rank, int node_size, cl_command_queue queue, 
			 cl_kernel kernel, cl_context context)
{
    int i, j, k;
	int local_start, local_end;
	int A_disp[node_size];
	int A_cnts[node_size];
	int C_disp[node_size];
	int C_cnts[node_size];
	
	cl_int status = CL_SUCCESS;
	cl_mem cl_A_buf;
	cl_mem cl_B_buf;
	cl_mem cl_C_buf;
	cl_mem cl_subA, cl_subC;
	
	// calculate counts and displacements for C
	for (i = 0; i < node_size; ++i)
		C_disp[i] = (i * ceil(nx/(float)node_size)) * nz;
	for (i = 0; i < node_size-1; ++i)
		C_cnts[i] = C_disp[i+1] - C_disp[i];
	C_cnts[node_size-1] = (nx * nz) - C_disp[node_size-1];

	// round count for send complete of A fragment
	for (i = 0; i < node_size; ++i)
		A_disp[i] = (i * ceil(nx/(float)node_size)) * ny;
	for (i = 0; i < node_size-1; ++i)
		A_cnts[i] = A_disp[i+1] - A_disp[i];
	A_cnts[node_size-1] = (nx * ny) - A_disp[node_size-1];
	
	// calculate local start and end
	local_start = rank * ceil(nx/(float)node_size) * nz;
	local_end = (rank+1) * ceil(nx/(float)node_size) * nz;
	if (local_end > nx*nz) local_end = nx*nz;
    
    const size_t work_group_size = 64;
	const size_t global_work_size = round_to((local_end - local_start + 1), 
	                                         work_group_size);	
	const size_t group_num = global_work_size/work_group_size;
	
	MPI_Scatterv((void*)A, A_cnts, A_disp, MPI_DOUBLE, 
		(void*)(A+A_disp[rank]), A_cnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	MPI_Bcast(B, (ny * nz), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    cl_A_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, nx * ny * sizeof(double), NULL, &status);
	cl_B_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, ny * nz * sizeof(double), NULL, &status);
	cl_C_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, nx * nz * sizeof(double), NULL, &status);
	
	cl_buffer_region A_info;
	A_info.origin = (size_t)(A_disp[rank] * sizeof(double));
	A_info.size = (size_t)(A_cnts[rank] * sizeof(double));
	cl_buffer_region C_info;
	C_info.origin = (size_t)(C_disp[rank] * sizeof(double));
	C_info.size = (size_t)(C_cnts[rank] * sizeof(double));
    
    cl_subA = clCreateSubBuffer(cl_A_buf, CL_MEM_READ_ONLY, CL_BUFFER_CREATE_TYPE_REGION, &A_info, &status);
	cl_subC = clCreateSubBuffer(cl_C_buf, CL_MEM_WRITE_ONLY, CL_BUFFER_CREATE_TYPE_REGION, &C_info, &status);
	
	status = clEnqueueWriteBuffer(queue, cl_subA, CL_TRUE, 0, A_cnts[rank] * sizeof(double), 
								  A + A_disp[rank], 0, NULL, NULL);
	status = clEnqueueWriteBuffer(queue, cl_B_buf, CL_TRUE, 0, 
	                              ny * nz * sizeof(double), B, 0, NULL, NULL);
	clFinish(queue);
	
	// set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_A_buf);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_B_buf);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &cl_C_buf);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel, 3, sizeof(int), (void*)&nx);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel, 4, sizeof(int), (void*)&ny);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(kernel, 5, sizeof(int), (void*)&nz);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");
	status = clSetKernelArg(kernel, 6, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 6");
	status = clSetKernelArg(kernel, 7, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 7");	
	
	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH KERNEL");
	clFinish(queue);
	
	status = clEnqueueReadBuffer(queue, cl_subC, CL_TRUE, 0, C_cnts[rank] * sizeof(double), 
							     C + C_disp[rank], 0, NULL, NULL);
	clFinish(queue);
    
    MPI_Gatherv((void*)(C+C_disp[rank]), C_cnts[rank], MPI_DOUBLE,
		(void*)C, C_cnts, C_disp, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
	clReleaseMemObject(cl_A_buf);
	clReleaseMemObject(cl_B_buf);
	clReleaseMemObject(cl_C_buf);
}

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

int main(int argc, char* argv[])
{
    int i, j, it;
    double meanA = 0.0f;
    double meanB = 0.0f;
    double meanC = 0.0f;
    long long time0, time1;

	int rank, node_size;

	cl_int status = CL_SUCCESS;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	cl_program program;
	cl_kernel kernel;
	cl_mem cl_sum_buffer;
	
	// initial cluster
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &node_size);

	if (argc != 5)
	{
		if (rank == 0)
			printf("Usage: %s <nx> <ny> <ns> <nt>\n", argv[0]);
		exit(1);
	}

	parse_arg(nx, argv[1]);
	parse_arg(ny, argv[2]);
	parse_arg(ns, argv[3]);
	parse_arg(nt, argv[4]);
	
	// broadcast important data
	MPI_Bcast(&nx, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ny, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ns, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&nt, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	// initial GPU
	status = clGetPlatformIDs(1, &platform, NULL);
	_GfnCheckCLStatus(status, "clGetPlatformIDs");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	_GfnCheckCLStatus(status, "clGetDeviceIDs");	
	context = clCreateContext(0, 1, &device, NULL, NULL, &status);
	_GfnCheckCLStatus(status, "clCreateContext");
	queue = clCreateCommandQueue(context, device, /*0*/CL_QUEUE_PROFILING_ENABLE, &status);
	_GfnCheckCLStatus(status, "clCreateCommandQueue");
	
	// create kernel
	program = clCreateProgramWithSource(context, 1, &prog_src, NULL, &status);
	_GfnCheckCLStatus(status, "CREATE PROGRAM WITH SOURCE");
	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	_GfnCheckCLStatus(status, "BUILD PROGRAM");
	status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
        LOG_SIZE, param_value, &param_value_size_ret);
    if (param_value_size_ret != 2)
        printf("Message from kernel compiler : \n%s\n", param_value);
	kernel = clCreateKernel(program, "gpu_matmul", &status);
	_GfnCheckCLStatus(status, "CREATE KERNEL");
	
	if (rank == 0)
		printf("nx = %d , ny = %d, ns = %d, nt = %d", nx, ny, ns, nt);

	double** A = (double**)malloc(nx * sizeof(double*));
	A[0] = (double*)malloc(nx * ny * sizeof(double));
	for (i = 1; i < nx; i++) A[i] = A[i-1] + ny;
	double** B = (double**)malloc(ny * sizeof(double*));
	B[0] = (double*)malloc(ny * ns * sizeof(double));
	for (i = 1; i < ny; i++) B[i] = B[i-1] + ns;
	double** C = (double**)malloc(nx * sizeof(double*));
	C[0] = (double*)malloc(nx * ns * sizeof(double));
	for (i = 1; i < nx; i++) C[i] = C[i-1] + ns;

	if (!A || !B || !C)
	{
		printf("Error rank %d allocating memory for arrays: %p, %p, %p\n", 
			rank, A, B, C);
		exit(1);
	}

	if (rank == 0)
	{
		for (i = 0; i < nx; i++)
		{
			for (j = 0; j < ny; j++)
			{
				A[i][j] = double_rand();
				meanA += A[i][j];
			}
		}
		for (i = 0; i < ny; i++)
		{
			for (j = 0; j < ns; j++)
			{
				B[i][j] = double_rand();
				meanB += B[i][j];
			}
		}
		printf("initial mean = %f\n", (meanA / (nx * ny) + meanB / (ny * ns)));
	}

	// warm up
	matmul_(nx, ny, ns, A[0], B[0], C[0], rank, node_size,
					  queue, kernel, context);
	
	time0 = get_time();
	for (it = 0; it < nt; it++)
		matmul_(nx, ny, ns, A[0], B[0], C[0], rank, node_size,
					  queue, kernel, context);
    time1 = get_time();

	// For the final mean - account only the norm of the top
	// most level (tracked by swapping idxs array of indexes).
	if (rank == 0)
	{
		for (i = 0; i < ny; i++)
	    	for (j = 0; j < ns; j++)
		    	meanC += C[i][j];
		printf("final mean = %f\n", meanC / (ny * ns));
	
		printf("total time = %f sec.\n", (float)(time1-time0)/1000000);
		printf("average time = %f sec.\n", ((float)(time1-time0)/1000000)/nt);
	}

    free(A[0]);
	free(A);
	free(B[0]);
	free(B);
	free(C[0]);
	free(C);
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	MPI_Finalize();

	return 0;
}

