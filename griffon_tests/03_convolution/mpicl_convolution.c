#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void gpu_convolution(__global double *matrix,					\n"
"                              __global double *result_matrix,			\n"
"							   int N,									\n"
"							   int start,								\n"
"							   int end) 								\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	int i, j, m, n;														\n"
"	double new_val;														\n"
"	if (tid < end) {													\n"
"		i = tid / N;													\n"
"		j = tid % N;													\n"
"		if (i >= 60 && i < N-60 && j >= 60 && j < N-60) {				\n"
"			new_val = 0.0;												\n"
"			for (m = 0; m < 121; ++m) {									\n"
"				for (n = 0; n < 121; ++n) {								\n"
"					new_val += matrix[(i+m-60)*N+(j+n-60)];				\n"
"				}														\n"
"			}															\n"
"			result_matrix[i*N+j] = new_val / 14641.0;					\n"
"		} else {														\n"
"			result_matrix[i*N+j] = matrix[i*N+j];						\n"
"		}																\n"
"	}																	\n"
"}																		\n"
"																		\n"
"__kernel void gpu_copy(__global double *matrix,						\n"
"                       __global double *result_matrix,					\n"
"						int start,										\n"
"					    int end) 										\n"
"{																		\n"
"	int tid = get_global_id(0) + start;									\n"
"	if (tid < end) {													\n"
"		matrix[tid] = result_matrix[tid];								\n"
"	}																	\n"
"}																		\n"
;

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

#define EPSILON 0.1
int is_equal(double a, double b) {
	return (a+EPSILON > b && b > a-EPSILON);
}

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

void init(int N, double **mat) {
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			/* Random from 0.00 - 0.99 */
			mat[i][j] = (rand() % 100) / 100.f;
}

void print_matrix(int N, double *mat) {
	int i, j;

	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j)
			printf("%3.3f ", mat[i*N+j]);
		printf("\n");
	}
}

void copy_matrix(int N, double **src, double **dsc) {
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			dsc[i][j] = src[i][j];
}

void copy_bound(int size, double *src, double *dsc) {
	int i;

	for (i = 0; i < size; ++i)
		dsc[i] = src[i];
}

void convolution_kernel(int N, int iterator, double *matrix, double *result_matrix,
						int rank, int node_size, cl_command_queue queue, 
				   		cl_kernel kernel, cl_kernel cpy_kernel, cl_context context)
{
	int i, j, m, n, tid;
	int local_start, local_end;
	int disp[node_size];
	int cnts[node_size];
	int it;

	cl_int status = CL_SUCCESS;
	cl_mem cl_matrix;
	cl_mem cl_result_matrix;
	cl_mem cl_submat;
	cl_mem cl_h2d_lower;
	cl_mem cl_h2d_upper;
	cl_mem cl_d2h_lower;
	cl_mem cl_d2h_upper;

	MPI_Status mstatus;
	MPI_Request send_lower_req;
	MPI_Request send_upper_req;
	MPI_Request recv_lower_req;
	MPI_Request recv_upper_req;

	// calculate counts and displacements
	for (i = 0; i < node_size; ++i)
		disp[i] = i * ceil(N/(double)node_size) * N/*blocksize*/;
	for (i = 0; i < node_size-1; ++i)
		cnts[i] = disp[i+1] - disp[i];
	cnts[node_size-1] = (N*N) - disp[node_size-1];

	// calculate local start and end
	local_start = rank * ceil(N*N/(double)node_size);
	local_end = (rank+1) * ceil(N*N/(double)node_size);
	if (local_end > N*N) local_end = N*N;

	const size_t work_group_size = 64;
	const size_t global_work_size = round_to(local_end - local_start + 1, 
	                                         work_group_size);	
	const size_t group_num = global_work_size/work_group_size;

	cl_matrix = clCreateBuffer(context, CL_MEM_READ_WRITE, N * N * sizeof(double), NULL, &status);
	cl_result_matrix = clCreateBuffer(context, CL_MEM_READ_WRITE, N * N * sizeof(double), NULL, &status);

	// scatter matrix
	MPI_Scatterv((void*)matrix, cnts, disp, MPI_DOUBLE,
		(void*)(matrix+disp[rank]), cnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// create subbuffer
	cl_buffer_region sub_matrix_info;
	sub_matrix_info.origin = (size_t)(disp[rank] * sizeof(double));
	sub_matrix_info.size = (size_t)(cnts[rank] * sizeof(double));
	cl_submat = clCreateSubBuffer(cl_matrix, CL_MEM_READ_WRITE, 
		CL_BUFFER_CREATE_TYPE_REGION, &sub_matrix_info, &status);

	// send data to GPU
	//t0 = get_time();
	status = clEnqueueWriteBuffer(queue, cl_submat, CL_FALSE, 0, cnts[rank] * sizeof(double),
		matrix+disp[rank], 0, NULL, NULL);

	// create download upper bound subbuffer
	cl_buffer_region d2h_upper_info;
	if (rank != 0) {
		d2h_upper_info.origin = (size_t)(disp[rank] * sizeof(double));
		d2h_upper_info.size = (size_t)(60*N * sizeof(double));
		cl_d2h_upper = clCreateSubBuffer(cl_matrix, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_upper_info, &status);
	}

	// create download lower bound subbuffer
	cl_buffer_region d2h_lower_info;
	if (rank != (node_size-1)) {
		d2h_lower_info.origin = (size_t)((disp[rank+1]-(60*N)) * sizeof(double));
		d2h_lower_info.size = (size_t)(60*N * sizeof(double));
		cl_d2h_lower = clCreateSubBuffer(cl_matrix, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &d2h_lower_info, &status);
	}

	// create upload upper bound subbuffer
	cl_buffer_region h2d_upper_info;
	if (rank != 0) {
		h2d_upper_info.origin = (size_t)((disp[rank]-(60*N)) * sizeof(double));
		h2d_upper_info.size = (size_t)(60*N * sizeof(double));
		cl_h2d_upper = clCreateSubBuffer(cl_matrix, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_upper_info, &status);
	}

	// create upload lower bound subbuffer
	cl_buffer_region h2d_lower_info;
	if (rank != (node_size-1)) {
		h2d_lower_info.origin = (size_t)(disp[rank+1] * sizeof(double));
		h2d_lower_info.size = (size_t)(60*N * sizeof(double));
		cl_h2d_lower = clCreateSubBuffer(cl_matrix, CL_MEM_READ_WRITE,
			CL_BUFFER_CREATE_TYPE_REGION, &h2d_lower_info, &status);
	}

	// wait write buffer
	clFinish(queue);


	// set kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_matrix);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_result_matrix);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel, 2, sizeof(int), (void*)&N);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel, 3, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel, 4, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");

	// set copy kernel arguments
	status = clSetKernelArg(cpy_kernel, 0, sizeof(cl_mem), &cl_matrix);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(cpy_kernel, 1, sizeof(cl_mem), &cl_result_matrix);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(cpy_kernel, 2, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(cpy_kernel, 3, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");

// run kernel
for (it = 0; it < iterator; it++) {

	// download lower bound from GPU
	if (rank != (node_size-1)) {
		status = clEnqueueReadBuffer(queue, cl_d2h_lower, CL_TRUE, 0, 60*N * sizeof(double), 
			matrix+(disp[rank+1]-(60*N)), 0, NULL, NULL);
	}

	// download upper bound from GPU
	if (rank != 0) {
		status = clEnqueueReadBuffer(queue, cl_d2h_upper, CL_TRUE, 0, 60*N * sizeof(double), 
			matrix+disp[rank], 0, NULL, NULL);
	}

	// exchange matrix bound
	/*int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request)*/
	if (rank != (node_size-1))
		MPI_Isend((void*)(matrix+disp[rank+1]-(60*N)), 60*N, MPI_DOUBLE, rank+1, 
			0/*tag*/, MPI_COMM_WORLD, &send_lower_req);
	if (rank != 0)
		MPI_Isend((void*)(matrix+disp[rank]), 60*N, MPI_DOUBLE, rank-1, 
			1/*tag*/, MPI_COMM_WORLD, &send_upper_req);

	/*int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request *request)*/
	if (rank != (node_size-1))
		MPI_Irecv((void*)(matrix+disp[rank+1]), 60*N, MPI_DOUBLE, rank+1,
			1/*tag*/, MPI_COMM_WORLD, &recv_lower_req);
	if (rank != 0)
		MPI_Irecv((void*)(matrix+disp[rank]-(60*N)), 60*N, MPI_DOUBLE, rank-1,
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
		status = clEnqueueWriteBuffer(queue, cl_h2d_upper, CL_TRUE, 0, 60*N * sizeof(double),
			matrix+disp[rank]-(60*N), 0, NULL, NULL);
	}

	// upload lower bound to GPU
	if (rank != (node_size-1)) {
		status = clEnqueueWriteBuffer(queue, cl_h2d_lower, CL_TRUE, 0, 60*N * sizeof(double),
			matrix+disp[rank+1], 0, NULL, NULL);
	}

	// launch kernel
	//t2 = get_time();
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

	// copy back sum buffer
	//t4 = get_time();
	status = clEnqueueReadBuffer(queue, cl_submat, CL_TRUE, 0, cnts[rank] * sizeof(double), 
		matrix+disp[rank], 0, NULL, NULL);
	//clFinish(queue);
	//t5 = get_time();

	MPI_Gatherv((void*)(matrix+disp[rank]), cnts[rank], MPI_DOUBLE,
		(void*)matrix, cnts, disp, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	clReleaseMemObject(cl_matrix);
	clReleaseMemObject(cl_result_matrix);
}

int main(int argc, char *argv[]) {
	int i, j, m, n, tid;
	int N = 1500, ite = 1;
	int it, iterator = 10;
	double **matrix, **result_matrix;
	double **verify_matrix, **orig_mat;
	long long time0, time1;
	
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

	if (rank == 0) {
		N = 1500;
		ite = 1;

		if (argc > 1) N = atoi(argv[1]);
		if (argc > 2) ite = atoi(argv[2]);
	}

	// broadcast important data
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
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
        
	kernel = clCreateKernel(program, "gpu_convolution", &status);
	_GfnCheckCLStatus(status, "CREATE CONVOLUTION KERNEL");
	cpy_kernel = clCreateKernel(program, "gpu_copy", &status);
	_GfnCheckCLStatus(status, "CREATE COPY KERNEL");

	// allocate matrix memory 
	matrix = (double **) malloc(N * sizeof(double*));
	matrix[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		matrix[i] = matrix[i-1] + N;
	result_matrix = (double **) malloc(N * sizeof(double*));
	result_matrix[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		result_matrix[i] = result_matrix[i-1] + N;
	orig_mat = (double **) malloc(N * sizeof(double*));
	orig_mat[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		orig_mat[i] = orig_mat[i-1] + N;
	verify_matrix = (double **) malloc(N * sizeof(double*));
	verify_matrix[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		verify_matrix[i] = verify_matrix[i-1] + N;

	// initialize matrix
	if (rank == 0)
		init(N, orig_mat);

	// warm up
	copy_matrix(N, orig_mat, matrix);
	convolution_kernel(N, iterator, matrix[0], result_matrix[0],
		rank, node_size, queue, kernel, cpy_kernel, context);

	for (i = 0; i < ite; i++) {
		copy_matrix(N, orig_mat, matrix);
		time0 = get_time();
		convolution_kernel(N, iterator, matrix[0], result_matrix[0],
			rank, node_size, queue, kernel, cpy_kernel, context);
		time1 = get_time();
	}

	// assert and show result
	if (rank == 0) {
		int pass = 1;

		/*for (it = 0; it < iterator; it++) {
		// apply filter
		for (tid = 0; tid < N*N; ++tid) {
			i = tid / N;
			j = tid % N;
			if (i >= 60 && i < N-60 && j >= 60 && j < N-60) {
				double new_val = 0.0;
				for (m = 0; m < 121; ++m) {	
					for (n = 0; n < 121; ++n) {	
						new_val += orig_mat[i+m-60][j+n-60];
					}
				}
				verify_matrix[i][j] = new_val / 14641.0;
			} else {
				verify_matrix[i][j] = orig_mat[i][j];
			}
		}


		// copy result to original matrix
		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				if (i >= 60 && i < N-60 && j >= 60 && j < N-60) {
					orig_mat[i][j] = verify_matrix[i][j];
				}
			}
		}
		}

		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				if (fabs(orig_mat[i][j]-matrix[i][j]) > 0.1) {
					printf("Error at [%d][%d]\n", i, j);
					printf("Actual value is %.5f but expected value is %.5f\n",
						matrix[i][j], orig_mat[i][j]);
					pass = 0;
					break;
				}
			}
		}*/

		printf("TEST 03 - Convolution\n");
		printf("\tTest result = ");
		if (pass) printf("SUCCESSFUL\n"); else printf("FAILURE\n");
		printf("\tProblem size = %d x %d\n", N, N);
		printf("\tRunning iteration = %d\n", ite);
		printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	}

	free(matrix[0]);
	free(matrix);
	free(orig_mat[0]);
	free(orig_mat);
	free(result_matrix[0]);
	free(result_matrix);
	free(verify_matrix[0]);
	free(verify_matrix);

	clReleaseKernel(kernel);
	clReleaseKernel(cpy_kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	MPI_Finalize();

	return 0;
}
