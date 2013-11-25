#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void gpu_convolution() {										\n"
"}																		\n"
;

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

#define EPSILON 0.01
int is_equal(float a, float b) {
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

void init(int N, float **mat) {
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			/* Random from 0.00 - 0.99 */
			mat[i][j] = (rand() % 100) / 100.f;
}

void copy_matrix(int N, float **src, float **dsc) {
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			dsc[i][j] = src[i][j];
}

void copy_bound(int size, float *src, float *dsc) {
	int i;

	for (i = 0; i < size; ++i)
		dsc[i] = src[i];
}

void convolution_kernel(int N, int iterator, float *matrix,
						int filterN, float *filter,
						int rank, int node_size, cl_command_queue queue, 
				   		cl_kernel kernel, cl_context context)
{
	int i, j, m, n;
	int local_start, local_end;
	int disp[node_size];
	int cnts[node_size];
	float *lower_bound_tmp;
	float *upper_bound_tmp;
	int it;

	// allocate boundary temp
	lower_bound_tmp = (float*) malloc(N*2*sizeof(float));
	upper_bound_tmp = (float*) malloc(N*2*sizeof(float));

	// calculate counts and displacements
	for (i = 0; i < node_size; ++i)
		disp[i] = i * ceil(n/(float)node_size) * n/*blocksize*/;
	for (i = 0; i < node_size-1; ++i)
		cnts[i] = disp[i+1] - disp[i];
	cnts[node_size-1] = (n*n) - disp[node_size-1];

	// calculate local start and end
	local_start = rank * ceil(n/(float)node_size);
	local_end = (rank+1) * ceil(n/(float)node_size);
	if (local_end > n) local_end = n;

	// scatter matrix
	MPI_Scatterv((void*)matrix, cnts, disp, MPI_FLOAT,
		(void*)(matrix+disp[rank]), cnts[rank], MPI_FLOAT, 0, MPI_COMM_WORLD);

	// boardcast filter
	MPI_Bcast(filter, filterN*filterN, MPI_FLOAT, 0, MPI_COMM_WORLD);

// run kernel
for (it = 0; it < iterator; it++) {

	int istart = (rank == 0) ? 2 : local_start;
	int iend = (rank == node_size-1) ? N-2 : local_end;

	for (i = istart; i < iend; ++i) {	
		for (j = 0+2; j < N-2; ++j) {
			float new_val = 0.0;
			for (m = 0; m < filterN; ++m) {
				for (n = 0; n < filterN; ++n) {
					new_val += (filter[m*filterN+n] * matrix[(i+m-2)*N+(j+n-2)]);
				}
			}
			matrix[i*N+j] = new_val;
		}
	}

	MPI_Status mstatus;
	MPI_Request send_lower_req;
	MPI_Request send_upper_req;
	MPI_Request recv_lower_req;
	MPI_Request recv_upper_req;

	// exchange matrix bound
#if 0
	/*int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request)*/
	if (rank != (node_size-1))
		MPI_Isend((void*)matrix+disp[rank+1]-(2*N), 2*N, MPI_FLOAT, rank+1, 
			0/*tag*/, MPI_COMM_WORLD, &send_lower_req);
	if (rank != 0)
		MPI_Isend((void*)matrix+disp[rank], 2*N, MPI_FLOAT, rank-1, 
			1/*tag*/, MPI_COMM_WORLD, &send_upper_req);

	/*int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request *request)*/
	if (rank != (node_size-1))
		MPI_Irecv((void*)lower_bound_tmp, 2*N, MPI_FLOAT, rank+1,
			1/*tag*/, MPI_COMM_WORLD, &recv_lower_req);
	if (rank != 0)
		MPI_Irecv((void*)upper_bound_tmp, 2*N, MPI_FLOAT, rank-1,
			0/*tag*/, MPI_COMM_WORLD, &recv_upper_req);

	/*int MPI_Wait(MPI_Request *request, MPI_Status *status)*/
	if (rank != 0) {
		MPI_Wait(&send_upper_req, &mstatus);
		if (1/*send_upper_req*/)
			copy_bound(2*N, upper_bound_tmp, matrix+disp[rank]);
	}
	if (rank != (node_size-1)) {
		MPI_Wait(&send_lower_req, &mstatus);
		if (1/*send_lower_req*/)
			copy_bound(2*N, lower_bound_tmp, matrix+disp[rank+1]-(2*N));
	}
	if (rank != (node_size-1))
		MPI_Wait(&recv_lower_req, &mstatus);
	if (rank != 0)
		MPI_Wait(&recv_upper_req, &mstatus);
#endif
}

	MPI_Gatherv((void*)(matrix+disp[rank]), cnts[rank], MPI_FLOAT,
		(void*)matrix, cnts, disp, MPI_FLOAT, 0, MPI_COMM_WORLD);

	free(lower_bound_tmp);
	free(upper_bound_tmp);
}

int main(int argc, char *argv[]) {
	int i, j, m, n;
	int N = 1500, ite = 1;
	int it, iterator = 10;
	float **matrix, **orig_mat;
	float filter[5][5] = {
		{ 1/256.0,  4/256.0,  6/256.0,  4/256.0, 1/256.0 },
		{ 4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0 },
		{ 6/256.0, 24/256.0, 36/256.0, 24/256.0, 6/256.0 },
		{ 4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0 },
		{ 1/256.0,  4/256.0,  6/256.0,  4/256.0, 1/256.0 }
	};
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
		N = 1500;
		ite = 3;

		if (argc > 1) N = atoi(argv[1]);
		if (argc > 2) ite = atoi(argv[2]);
	}

	// boardcast important data
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
	_GfnCheckCLStatus(status, "CREATE KERNEL");

	// allocate matrix memory 
	matrix = (float **) malloc(N * sizeof(float*));
	matrix[0] = (float *) malloc(N * N * sizeof(float));
	for (i = 1; i < N; i++)
		matrix[i] = matrix[i-1] + N;
	orig_mat = (float **) malloc(N * sizeof(float*));
	orig_mat[0] = (float *) malloc(N * N * sizeof(float));
	for (i = 1; i < N; i++)
		orig_mat[i] = orig_mat[i-1] + N;

	// initialize matrix
	if (rank == 0)
		init(N, orig_mat);

	// warm up
	copy_matrix(N, orig_mat, matrix);
	convolution_kernel(N, iterator, matrix[0], 5, (void*)filter,
		rank, node_size, queue, kernel, context);

	time0 = get_time();
	for (i = 0; i < ite; i++) {
		copy_matrix(N, orig_mat, matrix);
		convolution_kernel(N, iterator, matrix[0], 5, (void*)filter,
			rank, node_size, queue, kernel, context);
	}
	time1 = get_time();

	// assert and show result
	if (rank == 0) {
		int pass = 1;

		for (it = 0; it < iterator; it++) {
		for (i = 0+2; i < N-2; ++i) {	
			for (j = 0+2; j < N-2; ++j) {
				float new_val = 0.0;
				for (m = 0; m < 5; ++m) {
					for (n = 0; n < 5; ++n) {
						new_val += (filter[m][n] * orig_mat[i+m-2][j+n-2]);
					}
				}
				orig_mat[i][j] = new_val;
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
		}

		printf("TEST 03 - Convolution\n");
		printf("\tTest result = ");
		if (pass) printf("SUCCESSFUL\n"); else printf("FAILURE\n");
		printf("\tProblem size = %d x %d\n", N, N);
		printf("\tRunning iteration = %d\n", ite);
		printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	}

	free(matrix[0]);
	free(matrix);

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	MPI_Finalize();

	return 0;
}
 