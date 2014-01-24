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

#define _DEBUG

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
	name = atoi(arg); \
	if (name < 0) \
	{ \
		printf("Value for " #name " is invalid: %d\n", name); \
		exit(1); \
	}

#define double_rand() (((double)(rand() / (double)RAND_MAX) - 0.5) * 2)

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

#define EPSILON 0.1
int is_equal(double a, double b) {
	return (a+EPSILON > b && b > a-EPSILON);
}

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void gpu_convolution(__global double *w0,						\n"
"                              __global double *w1,						\n"
"							   int nx,									\n"
"							   int ny,									\n"
"							   int start,								\n"
"							   int end, 								\n"
"							   double s0, 								\n"
"							   double s1, 								\n"
"							   double s2, 								\n"
"							   double s4, 								\n"
"							   double s5, 								\n"
"							   double s8, 								\n"
"							   double f) 								\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j;															\n"
"	if (tid < end) {													\n"
"		i = tid / ny;													\n"
"		j = tid % ny;													\n"
"		if (i >= 2 && i < nx-2 && j >= 2 && j < ny-2)					\n"
"w1[i*ny+j] = f * (														\n"
"	s0 * w0[i*ny+j] +													\n"
"	s1 * (w0[(i-1)*ny+ j ] + w0[(i+1)*ny+ j ] + w0[ i *ny+(j-1)] + w0[ i *ny+(j+1)]) + 				\n"
"	s2 * (w0[(i-1)*ny+(j-1)] + w0[(i+1)*ny+(j-1)] + w0[(i-1)*ny+(j+1)] + w0[(i+1)*ny+(j+1)]) +		\n"
"	s4 * (w0[(i-2)*ny+ j ] + w0[(i+2)*ny+ j ] + w0[ i *ny+(j-2)] + w0[ i *ny+(j+2)]) +				\n"
"	s5 * (w0[(i-2)*ny+(j-1)] + w0[(i-1)*ny+(j-2)] + w0[(i+1)*ny+(j-2)] + w0[(i+2)*ny+(j-1)]  +		\n"
"		  w0[(i-2)*ny+(j+1)] + w0[(i-1)*ny+(j+2)] + w0[(i+1)*ny+(j+2)] + w0[(i+2)*ny+(j+1)]) +		\n"
"	s8 * (w0[(i-2)*ny+(j-2)] + w0[(i+2)*ny+(j-2)] + w0[(i-2)*ny+(j+2)] + w0[(i+2)*ny+(j+2)]));		\n"
"	}																	\n"
"}																		\n"
"																		\n"
"__kernel void gpu_copy(__global double *w0,							\n"
"                       __global double *w1,							\n"
"						int start,										\n"
"					    int end) 										\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	if (tid < end) {													\n"
"		w0[tid] = w1[tid];												\n"
"	}																	\n"
"}																		\n"
;

cl_mem cl_w0, cl_sub_w0;
cl_mem cl_w1, cl_sub_w1;
size_t work_group_size;
size_t global_work_size;
size_t group_num;

void gaussblur(int nx, int ny,
	const double s0, const double s1, const double s2,
	const double s4, const double s5, const double s8,
	double** w0, double** w1,
	int rank, int node_size, cl_command_queue queue, 
	cl_kernel kernel, cl_kernel cpy_kernel, cl_context context,
	int local_start, int local_end,	int *disp, int *cnts)
{
	cl_int status = CL_SUCCESS;
	cl_mem cl_h2d_lower;
	cl_mem cl_h2d_upper;
	cl_mem cl_d2h_lower;
	cl_mem cl_d2h_upper;
	
	MPI_Status mstatus;
	MPI_Request send_lower_req;
	MPI_Request send_upper_req;
	MPI_Request recv_lower_req;
	MPI_Request recv_upper_req;

	// create download upper bound subbuffer
	cl_buffer_region d2h_upper_info;
	if (rank != 0) {
		d2h_upper_info.origin = (size_t)(disp[rank] * sizeof(double));
		d2h_upper_info.size = (size_t)(2 * ny * sizeof(double));
		cl_d2h_upper = clCreateSubBuffer(cl_w0, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_upper_info, &status);
	}

	// create download lower bound subbuffer
	cl_buffer_region d2h_lower_info;
	if (rank != (node_size-1)) {
		d2h_lower_info.origin = (size_t)((disp[rank+1]-(2 * ny)) * sizeof(double));
		d2h_lower_info.size = (size_t)(2 * ny * sizeof(double));
		cl_d2h_lower = clCreateSubBuffer(cl_w0, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_lower_info, &status);
	}

	// create upload upper bound subbuffer
	cl_buffer_region h2d_upper_info;
	if (rank != 0) {
		h2d_upper_info.origin = (size_t)((disp[rank]-(2 * ny)) * sizeof(double));
		h2d_upper_info.size = (size_t)(2 * ny * sizeof(double));
		cl_h2d_upper = clCreateSubBuffer(cl_w0, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_upper_info, &status);
	}

	// create upload lower bound subbuffer
	cl_buffer_region h2d_lower_info;
	if (rank != (node_size-1)) {
		h2d_lower_info.origin = (size_t)(disp[rank+1] * sizeof(double));
		h2d_lower_info.size = (size_t)(2 * ny * sizeof(double));
		cl_h2d_lower = clCreateSubBuffer(cl_w0, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_lower_info, &status);
	}
	
	// download lower bound from GPU
	if (rank != (node_size-1)) {
		status = clEnqueueReadBuffer(queue, cl_d2h_lower, CL_TRUE, 0, 2 * ny * sizeof(double), 
			(w0[0])+(disp[rank+1]-(2 * ny)), 0, NULL, NULL);
	}

	// download upper bound from GPU
	if (rank != 0) {
		status = clEnqueueReadBuffer(queue, cl_d2h_upper, CL_TRUE, 0, 2 * ny * sizeof(double), 
			(w0[0])+disp[rank], 0, NULL, NULL);
	}

	// exchange w0 bound
	/*int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request)*/
	if (rank != (node_size-1))
		MPI_Isend((void*)((w0[0])+disp[rank+1]-(2 * ny)), 2 * ny, MPI_DOUBLE, rank+1, 
			0/*tag*/, MPI_COMM_WORLD, &send_lower_req);
	if (rank != 0)
		MPI_Isend((void*)((w0[0])+disp[rank]), 2 * ny, MPI_DOUBLE, rank-1, 
			1/*tag*/, MPI_COMM_WORLD, &send_upper_req);

	/*int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request *request)*/
	if (rank != (node_size-1))
		MPI_Irecv((void*)((w0[0])+disp[rank+1]), 2 * ny, MPI_DOUBLE, rank+1,
			1/*tag*/, MPI_COMM_WORLD, &recv_lower_req);
	if (rank != 0)
		MPI_Irecv((void*)(w0[0]+disp[rank]-(2 * ny)), 2 * ny, MPI_DOUBLE, rank-1,
			0/*tag*/, MPI_COMM_WORLD, &recv_upper_req);

	/*int MPI_Wait(MPI_Request *request, MPI_Status *status)*/
	if (rank != 0)
		MPI_Wait(&send_upper_req, &mstatus);
	if (rank != (node_size-1))
		MPI_Wait(&send_lower_req, &mstatus);
	if (rank != (node_size-1))
		MPI_Wait(&recv_lower_req, &mstatus);
	if (rank != 0)
		MPI_Wait(&recv_upper_req, &mstatus);


	// upload upper bound to GPU
	if (rank != 0) {
		status = clEnqueueWriteBuffer(queue, cl_h2d_upper, CL_TRUE, 0, 2 * ny * sizeof(double),
			(w0[0])+disp[rank]-(2 * ny), 0, NULL, NULL);
	}

	// upload lower bound to GPU
	if (rank != (node_size-1)) {
		status = clEnqueueWriteBuffer(queue, cl_h2d_lower, CL_TRUE, 0, 2 * ny * sizeof(double),
			(w0[0])+disp[rank+1], 0, NULL, NULL);
	}

	// launch kernel
	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH CONVOLUTION KERNEL");
	clFinish(queue);
	//t3 = get_time();
	
	// launch copy kernel
	status = clEnqueueNDRangeKernel(queue, cpy_kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH COPY KERNEL");
	clFinish(queue);
}

int main(int argc, char* argv[])
{
    int i, j, it;
    int nx, ny, nt;
    double **w0, **w1;
    double s0, s1, s2, s4, s5, s8; 
    double mean = 0.0f;
    long long time0, time1;
    int local_start, local_end;
    double f;

	int rank, node_size;

	cl_int status = CL_SUCCESS;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	cl_program program;
	cl_kernel kernel;
	cl_kernel cpy_kernel;
	cl_mem cl_sum_buffer;

	// initial cluster
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &node_size);

	int disp[node_size];
	int cnts[node_size];

	if (argc != 4)
	{
		if (rank == 0)
			printf("Usage: %s <nx> <ny> <nt>\n", argv[0]);
		exit(1);
	}

	parse_arg(nx, argv[1]);
	parse_arg(ny, argv[2]);
	parse_arg(nt, argv[3]);
	
	MPI_Bcast(&nx, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ny, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&nt, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (rank == 0)
		printf("nx = %d , ny = %d , nt = %d\n", nx, ny, nt);

	s0 = double_rand();
	s1 = double_rand();
	s2 = double_rand();
	s4 = double_rand();
	s5 = double_rand();
	s8 = double_rand();
	
	MPI_Bcast(&s0, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s2, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s4, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s5, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s8, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("s0 = %f, s1 = %f, s2 = %f\n", s0, s1, s2);
		printf("s4 = %f, s5 = %f, s8 = %f\n", s4, s5, s8);
	}
	
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
        
	kernel = clCreateKernel(program, "gpu_convolution", &status);
	_GfnCheckCLStatus(status, "CREATE CONVOLUTION KERNEL");
	cpy_kernel = clCreateKernel(program, "gpu_copy", &status);
	_GfnCheckCLStatus(status, "CREATE COPY KERNEL");

	w0 = (double**)malloc(nx * sizeof(double*));
	w0[0] = (double*)malloc(nx * ny * sizeof(double));
	w1 = (double**)malloc(nx * sizeof(double*));
	w1[0] = (double*)malloc(nx * ny * sizeof(double));
	for (i = 1; i < nx; i++)
	{
	    w0[i] = w0[(i-1)] + ny;
	    w1[i] = w1[(i-1)] + ny;
	}

	if (!w0 || !w1)
	{
		if (rank == 0)
			printf("Error allocating memory for arrays: %p, %p\n", w0, w1);
		exit(1);
	}

	for (i = 0; i < nx; i++)
	{
	    for (j = 0; j < ny; j++)
	    {
		    w0[i][j] = double_rand();
		    w1[i][j] = double_rand();
		    mean += w0[i][j] + w1[i][j];
		}
	}
	if (rank == 0)
		printf("initial mean = %f\n", mean / (nx * ny) / 2);

	// calculate counts and displacements
	for (i = 0; i < node_size; ++i)
		disp[i] = i * ceil(nx/(double)node_size) * ny;
	for (i = 0; i < node_size-1; ++i)
		cnts[i] = disp[(i+1)] - disp[i];
	cnts[node_size-1] = (nx * ny) - disp[node_size-1];

	// calculate local start and end
	local_start = rank * ceil(nx/(double)node_size) * ny;
	local_end = (rank+1) * ceil(nx/(double)node_size) * ny;
	if (local_end > nx*ny) local_end = nx * ny;

	work_group_size = 64;
	global_work_size = round_to(local_end - local_start + 1, 
	                            work_group_size);	
	group_num = global_work_size/work_group_size;

	cl_w0 = clCreateBuffer(context, CL_MEM_READ_WRITE, nx * ny * sizeof(double), NULL, &status);
	cl_w1 = clCreateBuffer(context, CL_MEM_READ_WRITE, nx * ny * sizeof(double), NULL, &status);

	// scatter w0 and w1
	MPI_Scatterv((void*)(w0[0]), cnts, disp, MPI_DOUBLE,
		(void*)((w0[0])+disp[rank]), cnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatterv((void*)(w1[0]), cnts, disp, MPI_DOUBLE,
		(void*)((w1[0])+disp[rank]), cnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// create subbuffer
	cl_buffer_region sub_w_info;
	sub_w_info.origin = (size_t)(disp[rank] * sizeof(double));
	sub_w_info.size = (size_t)(cnts[rank] * sizeof(double));
	
	cl_sub_w0 = clCreateSubBuffer(cl_w0, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_w_info, &status);
	cl_sub_w1 = clCreateSubBuffer(cl_w1, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_w_info, &status);
		
	// send data to GPU
	status = clEnqueueWriteBuffer(queue, cl_sub_w0, CL_FALSE, 0, cnts[rank] * sizeof(double),
		(w0[0])+disp[rank], 0, NULL, NULL);
	status = clEnqueueWriteBuffer(queue, cl_sub_w1, CL_FALSE, 0, cnts[rank] * sizeof(double),
		(w0[0])+disp[rank], 0, NULL, NULL);
	clFinish(queue);
	
	f = 1. / (s0 + 4 * (s1 + s2 + s4 + s8) + 8 * s5);
	
	// set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_w0);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_w1);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel, 2, sizeof(int), (void*)&nx);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel, 3, sizeof(int), (void*)&ny);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel, 4, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(kernel, 5, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");
	status = clSetKernelArg(kernel, 6, sizeof(double), (void*)&s0);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 6");
	status = clSetKernelArg(kernel, 7, sizeof(double), (void*)&s1);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 7");
	status = clSetKernelArg(kernel, 8, sizeof(double), (void*)&s2);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 8");
	status = clSetKernelArg(kernel, 9, sizeof(double), (void*)&s4);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 9");
	status = clSetKernelArg(kernel, 10, sizeof(double), (void*)&s5);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 10");
	status = clSetKernelArg(kernel, 11, sizeof(double), (void*)&s8);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 11");
	status = clSetKernelArg(kernel, 12, sizeof(double), (void*)&f);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 12");
	
	// set copy kernel arguments
	status = clSetKernelArg(cpy_kernel, 0, sizeof(cl_mem), &cl_w0);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(cpy_kernel, 1, sizeof(cl_mem), &cl_w1);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(cpy_kernel, 2, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(cpy_kernel, 3, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");

    time0 = get_time();
	for (it = 0; it < nt; it++)
		gaussblur(nx, ny, s0, s1, s2, s4, s5, s8, w0, w1,
			rank, node_size, queue, kernel, cpy_kernel, context,
			local_start, local_end, disp, cnts);
	time1 = get_time();
	
	// copy back sum buffer
	status = clEnqueueReadBuffer(queue, cl_sub_w0, CL_TRUE, 0, cnts[rank] * sizeof(double), 
		(w0[0])+disp[rank], 0, NULL, NULL);

	MPI_Gatherv((void*)((w0[0])+disp[rank]), cnts[rank], MPI_DOUBLE,
		(void*)(w0[0]), cnts, disp, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// For the final mean - account only the norm of the top
	// most level (tracked by swapping idxs array of indexes).
	if (rank == 0)
	{
		mean = 0.0f;
		for (i = 0; i < nx; i++)
	    	for (j = 0; j < ny; j++)
	    		mean += w0[i][j];
		printf("final mean = %f\n", mean / (nx * ny));
	
		printf("total time = %f sec.\n", (float)(time1-time0)/1000000);
		printf("average time = %f sec.\n", ((float)(time1-time0)/1000000)/nt);
	}

    free(w0[0]);
	free(w0);
	free(w1[0]);
	free(w1);
	
	clReleaseMemObject(cl_w0);
	clReleaseMemObject(cl_w1);
	
	clReleaseKernel(kernel);
	clReleaseKernel(cpy_kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	MPI_Finalize();

	return 0;
}

