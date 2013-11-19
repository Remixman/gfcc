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

const char *prog_src = 
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable							\n"
"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable			\n"
"																		\n"
"__kernel void gpu_integrate(__global double *global_sum,    			\n"
"                            __local double *shared_sum,				\n"
"                            int n, double h,							\n"
"                            int start, int end) 						\n"
"{																		\n"
"	double x;															\n"
"	int gtid = get_global_id(0);										\n"
"	int ltid = get_local_id(0);											\n"
"	int i = gtid + start;												\n"
"	double sum = 0.0;													\n"
"																		\n"
"	if (i <= end) {														\n"
"		x = h * ((double) i - 0.5);										\n"
"		sum += 4.0 / (1.0 + x * x);										\n"
"	}																	\n"
"																		\n"
"	shared_sum[gtid] = sum;												\n"
"	for (int stride = get_local_size(0)/2; stride > 0; stride/=2) {		\n"
"		barrier(CLK_LOCAL_MEM_FENCE);									\n"
"		if (ltid < stride)												\n"
"			shared_sum[ltid] += shared_sum[ltid+stride];				\n"
"	}																	\n"
"																		\n"
"	barrier(CLK_LOCAL_MEM_FENCE);										\n"
"	if (ltid == 0)														\n"
"		global_sum[get_group_id(0)] = shared_sum[0];					\n"
"}																		\n"
;

#define LOG_SIZE (1024*1024)
size_t param_value_size_ret;
char param_value[LOG_SIZE];

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

double integrate(int n, int rank, int node_size) {
	double sum = 0, h, x;
	double global_sum = 0;
	int local_start, local_end;
	double *host_sum_buffer = NULL;
	int i;
	
	cl_int status = CL_SUCCESS;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	cl_program program;
	cl_kernel kernel;
	cl_mem cl_sum_buffer;
	
	h = 1.0 / (double) n;
	local_start = rank * (n/(double)node_size) + 1;
	local_end = (rank+1) * (n/(double)node_size);
	if (local_end > n) local_end = n;
	
	const size_t global_work_size = local_end - local_start + 1;
	const size_t work_group_size = 64;
	const size_t group_num = ceil(global_work_size/(double)work_group_size);
	
	printf("%d Debug\n", rank);
	printf("%d Global work size = %d\n", rank, (int)global_work_size);
	printf("%d Group num = %d\n", rank, (int)group_num);
	
	// initial GPU
	status = clGetPlatformIDs(1, &platform, NULL);
	_GfnCheckCLStatus(status, "clGetPlatformIDs");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	_GfnCheckCLStatus(status, "clGetDeviceIDs");
	context = clCreateContext(0, 1, &device, NULL, NULL, &status);
	_GfnCheckCLStatus(status, "clCreateContext");
	queue = clCreateCommandQueue(context, device, 0, &status);
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
	kernel = clCreateKernel(program, "gpu_integrate", &status);
	_GfnCheckCLStatus(status, "CREATE KERNEL");
	
	// create buffer
	cl_sum_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, group_num, NULL, &status);
	host_sum_buffer = (double*) malloc(group_num * sizeof(double));
	
	// set kernel argument
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_sum_buffer);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 0");
	status = clSetKernelArg(kernel, 1, sizeof(double) * work_group_size, 0);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 1");
	status = clSetKernelArg(kernel, 2, sizeof(int), (void*)&n);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 2");
	status = clSetKernelArg(kernel, 3, sizeof(double), (void*)&h);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 3");
	status = clSetKernelArg(kernel, 4, sizeof(int), (void*)&local_start);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 4");
	status = clSetKernelArg(kernel, 5, sizeof(int), (void*)&local_end);
	_GfnCheckCLStatus(status, "SET KERNEL ARG 5");

	/*for (i = local_start; i <= local_end; i++) {
		x = h*((double)i - 0.5);
		sum += 4.0 / (1.0 + x*x);
	}*/
	// launch kernel
	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size,
		&work_group_size, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "LAUNCH KERNEL");
		
	// copy back sum buffer
	clEnqueueReadBuffer(queue, cl_sum_buffer, CL_TRUE, 0, group_num, 
		host_sum_buffer, 0, NULL, NULL);
		
	// summation host_sum_buffer
	for (i = 0; i < group_num; ++i)
		sum += host_sum_buffer[i];
	
	MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	free(host_sum_buffer);
	clReleaseMemObject(cl_sum_buffer);
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
	return global_sum * h;
}

int main(int argc, char *argv[]) {
	int n, ite, i, elapse;
	double PI25DT = 3.14159265358;
	double pi;
	long long time0, time1;
	
	int rank, node_size;

	// initial cluster
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &node_size);

	if (rank == 0) {
		n = 2000000;
		ite = 10;

		if (argc > 1) n = atoi(argv[1]);
		if (argc > 2) ite = atoi(argv[2]);
	}

	// boardcast important data
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ite, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// warm up
	pi = integrate(n, rank, node_size);

	time0 = get_time();
	for(i = 0; i < ite; i++) pi = integrate(n, rank, node_size);
	time1 = get_time();
		
	if (rank == 0) {
		printf("last pi=%.16f, error=%.16f\n", pi, fabs(pi - PI25DT));

		printf("TEST 01 - Numerical Integration PI\n");
		printf("\tProblem size N = %d\n", n);
		printf("\tRunning iteration = %d\n", ite);
		printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	}

	MPI_Finalize();
	
	return 0;
}

