#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <CL/cl.h>

// http://parallelis.com/how-to-measure-opencl-kernel-execution-time/

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

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void gpu_matmul(__global float *A,							\n"
"                         __global float *B,							\n"
"                         __global float *C,							\n"
"                         int n,										\n"
"                         int start,									\n"
"                         int end)										\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j, k;														\n"
"																		\n"
"	if (tid < end) {													\n"
"		i = tid / n;													\n"
"		j = tid % n;													\n"
"		float tmp = 0.0;												\n"
"		for (k = 0; k < n; ++k) {										\n"
"			tmp += A[i*n+k] * B[k*n+j];									\n"
"		}																\n"
//"		printf(\"Set C[%d][%d] = %.5f\\n\", i, j, tmp);					\n"
"		C[i*n+j] = tmp;													\n"
"	}																	\n"
"}																		\n"
;

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

long long t0, t1, t2, t3, t4, t5;

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

int round_to(int x, int r) {
	int q = ceil(x/(float)r);
	return q * r;
}

void init(int n, float **A, float **B) {
	int i, j;
	
	srand(time(NULL));
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			/* Random from 0.00 - 0.99 */
			A[i][j] = (rand() % 100) / 100.f;
			B[i][j] = (rand() % 100) / 100.f;
		}
	}
}

void matmul_kernel(int n, float *A, float *B, float *C,
				   int rank, int node_size, cl_command_queue queue, 
				   cl_kernel kernel, cl_context context) 
{
	int i, j, k;
	int local_start, local_end;
	int A_disp[node_size];
	int A_cnts[node_size];
	int C_disp[node_size];
	int C_cnts[node_size];
	int nsquare = n*n;
	
	cl_int status = CL_SUCCESS;
	cl_mem cl_A_buf;
	cl_mem cl_B_buf;
	cl_mem cl_C_buf;
	cl_mem cl_subA, cl_subC;

	// calculate counts and displacements for C
	for (i = 0; i < node_size; ++i)
		C_disp[i] = i * ceil(nsquare/(float)node_size);
	for (i = 0; i < node_size-1; ++i)
		C_cnts[i] = C_disp[i+1] - C_disp[i];
	C_cnts[node_size-1] = nsquare - C_disp[node_size-1];

	// round count for send complete of A fragment
	for (i = 0; i < node_size; ++i)
		A_disp[i] = i * ceil(n/(float)node_size) * n;
	for (i = 0; i < node_size-1; ++i)
		A_cnts[i] = A_disp[i+1] - A_disp[i];
	A_cnts[node_size-1] = nsquare - A_disp[node_size-1];
	
	// calculate local start and end
	local_start = rank * ceil(nsquare/(float)node_size);
	local_end = (rank+1) * ceil(nsquare/(float)node_size);
	if (local_end > nsquare) local_end = nsquare;

	//printf("Rank %d : start %d , end %d\n", rank, local_start, local_end);

	const size_t work_group_size = 64;
	const size_t global_work_size = round_to(local_end - local_start + 1, 
	                                         work_group_size);	
	const size_t group_num = global_work_size/work_group_size;

	/*if (rank == 0) {
		for (i = 0; i < node_size; ++i) {
			printf("disp[%d] = %d\n", i, disp[i]);
			printf("cnts[%d] = %d\n", i, cnts[i]);
		}
		
		printf("%d Global work size = %d\n", rank, (int)global_work_size);
		printf("%d Group num = %d\n", rank, (int)group_num);
	}*/

	// scatter A
	MPI_Scatterv((void*)A, A_cnts, A_disp, MPI_FLOAT, 
		(void*)(A+A_disp[rank]), A_cnts[rank], MPI_FLOAT, 0, MPI_COMM_WORLD);

	// boardcast B
	MPI_Bcast(B, nsquare, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	cl_A_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, nsquare * sizeof(float), NULL, &status);
	cl_B_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, nsquare * sizeof(float), NULL, &status);
	cl_C_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, nsquare * sizeof(float), NULL, &status);
	
	cl_buffer_region A_info;
	A_info.origin = (size_t)(A_disp[rank] * sizeof(float));
	A_info.size = (size_t)(A_cnts[rank] * sizeof(float));
	cl_buffer_region C_info;
	C_info.origin = (size_t)(C_disp[rank] * sizeof(float));
	C_info.size = (size_t)(C_cnts[rank] * sizeof(float));
	
	cl_subA = clCreateSubBuffer(cl_A_buf, CL_MEM_READ_ONLY, CL_BUFFER_CREATE_TYPE_REGION, &A_info, &status);
	cl_subC = clCreateSubBuffer(cl_C_buf, CL_MEM_WRITE_ONLY, CL_BUFFER_CREATE_TYPE_REGION, &C_info, &status);
	
	// send data to GPU
	/*status = clEnqueueWriteBuffer(queue, cl_A_buf, CL_TRUE, 0, 
	                              sizeof(float) * n * n, A, 0, NULL, NULL);*/
	t0 = get_time();
	status = clEnqueueWriteBuffer(queue, cl_subA, CL_TRUE, 0, A_cnts[rank] * sizeof(float), 
								  A + A_disp[rank], 0, NULL, NULL);
	status = clEnqueueWriteBuffer(queue, cl_B_buf, CL_TRUE, 0, 
	                              nsquare * sizeof(float), B, 0, NULL, NULL);
	clFinish(queue);
	t1 = get_time();
	
	// set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_A_buf);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_B_buf);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &cl_C_buf);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel, 3, sizeof(int), (void*)&n);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel, 4, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(kernel, 5, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");	

	// launch kernel
	t2 = get_time();
	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH KERNEL");
	clFinish(queue);
	t3 = get_time();

	// copy back sum buffer
	t4 = get_time();
	status = clEnqueueReadBuffer(queue, cl_subC, CL_TRUE, 0, C_cnts[rank] * sizeof(float), 
							     C + C_disp[rank], 0, NULL, NULL);
	clFinish(queue);
	t5 = get_time();
	/*status = clEnqueueReadBuffer(queue, cl_C_buf, CL_TRUE, 0, 
	                             sizeof(float) * n * n, C, 0, NULL, NULL);*/
	
	// gather C
	MPI_Gatherv((void*)(C+C_disp[rank]), C_cnts[rank], MPI_FLOAT,
		(void*)C, C_cnts, C_disp, MPI_FLOAT, 0, MPI_COMM_WORLD);
		
	clReleaseMemObject(cl_A_buf);
	clReleaseMemObject(cl_B_buf);
	clReleaseMemObject(cl_C_buf);
}

int main(int argc, char *argv[]) {
	int ite, i, j, k;
	int n, pass = 1;
	float **A, **B, **C, sum;
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
	
	if (rank == 0) {
		n = 1000;
		ite = 10;

		if (argc > 1) n = atoi(argv[1]);
		if (argc > 2) ite = atoi(argv[2]);
	}
	
	// boardcast important data
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ite, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
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
	
	// allocate memory for A, B and C
	A = (float **) malloc(n * sizeof(float*));
	A[0] = (float *) malloc(n * n * sizeof(float));
	B = (float **) malloc(n * sizeof(float*));
	B[0] = (float *) malloc(n * n * sizeof(float));
	C = (float **) malloc(n * sizeof(float*));
	C[0] = (float *) malloc(n * n * sizeof(float));
	for (i = 1; i < n; i++) {
		A[i] = A[i-1] + n;
		B[i] = B[i-1] + n;
		C[i] = C[i-1] + n;
	}
	
	if (rank == 0) {
		init(n, A, B);
	}
	
	// warm up
	matmul_kernel(n, A[0], B[0], C[0], rank, node_size,
				  queue, kernel, context);

	time0 = get_time();
	for (i = 0; i < ite; i++) {
		matmul_kernel(n, A[0], B[0], C[0], rank, node_size,
					  queue, kernel, context);
	}
	time1 = get_time();
	
	if (rank == 0) {
		// assert with sequential solution
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				sum = 0.f;
				for (k = 0; k < n; k++) {
					sum += A[i][k] * B[k][j];
				}
				if (fabs(sum-C[i][j]) > 0.0001) {
					printf("Error at [%d][%d]\n", i, j);
					printf("C[%d][%d] is %.5f but expected value is %.5f\n",
						i, j, C[i][j], sum);
					pass = 0;
					break;
				}
			}
			if (!pass) break;
		}

		printf("Tranfer H2D time = %f sec.\n", (float)(t1-t0)/1000000);
		printf("Compute time = %f sec.\n", (float)(t3-t2)/1000000);
		printf("Tranfer D2H time = %f sec.\n", (float)(t5-t4)/1000000);

		printf("TEST 02 - Matrix Matrix Multiplication\n");
		printf("\tTest result = ");
		if (pass) printf("SUCCESSFUL\n"); else printf("FAILURE\n");
		printf("\tProblem size = %d x %d\n", n, n);
		printf("\tRunning iteration = %d\n", ite);
		printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	}

	free(A[0]); free(A);
	free(B[0]); free(B);
	free(C[0]); free(C);
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	MPI_Finalize();
	
	return 0;
}

