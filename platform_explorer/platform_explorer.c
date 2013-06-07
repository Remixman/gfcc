#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>
#include <CL/cl.h>

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

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

int main(int argc, char *argv[])
{
	cl_int status = CL_SUCCESS;   // Used to handle error codes
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	cl_program program;
	cl_kernel kernel;

	int crank, csize;

	long long time0, time1;
	size_t max_mem_size = sizeof(float) *  2500000;
	size_t transfer_size = sizeof(float) * 2500000;
	float *host_buffer = (float*) malloc(max_mem_size);
	cl_mem device_buffer;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &crank);
	MPI_Comm_size(MPI_COMM_WORLD, &csize);

	/* Test boardcast time */
	time0 = get_time();
	

	/* Initial platform on each node */
	status = clGetPlatformIDs(1, &platform, NULL);
	_GfnCheckCLStatus(status, "Getting platform id");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	_GfnCheckCLStatus(status, "Getting device id");
	context = clCreateContext(0, 1, &device, NULL, NULL, &status);
	_GfnCheckCLStatus(status, "Creating context");
	queue = clCreateCommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &status);
	_GfnCheckCLStatus(status, "Creating command queue");

if (crank == 0) {
	/* Test upload read only data */
	device_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, transfer_size, host_buffer, &status);
	_GfnCheckCLStatus(status, "Creating read only buffer");
	time0 = get_time();
	status = clEnqueueWriteBuffer(queue, device_buffer, CL_TRUE, 0, transfer_size, host_buffer, 0, NULL, NULL);
	time1 = get_time();
	_GfnCheckCLStatus(status, "Enqueue write buffer");
	//status = clFinish(queue);
	//_GfnCheckCLStatus(status, "Finish enqueue write buffer");
	clReleaseMemObject(device_buffer);
	printf("Time for upload read only data %zu bytes : %.10f\n", transfer_size, (float) (time1 - time0) / 1000000);
	
	/* Test upload read-write data */
	device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, transfer_size, host_buffer, &status);
	time0 = get_time();
	status = clEnqueueWriteBuffer(queue, device_buffer, CL_TRUE, 0, transfer_size, host_buffer, 0, NULL, NULL);
	time1 = get_time();
	clReleaseMemObject(device_buffer);
	printf("Time for upload read write data %zu bytes : %.10f\n", transfer_size, (float) (time1 - time0) / 1000000);

	/* Test download write only data */
	device_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, transfer_size, host_buffer, &status);
	time0 = get_time();
	status = clEnqueueReadBuffer(queue, device_buffer, CL_TRUE, 0, transfer_size, host_buffer, 0, NULL, NULL);
	time1 = get_time();
	clReleaseMemObject(device_buffer);
	printf("Time for download write only data %zu bytes : %.10f\n", transfer_size, (float) (time1 - time0) / 1000000);
}
		
	free(host_buffer);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	MPI_Finalize();

	return 0;
}

