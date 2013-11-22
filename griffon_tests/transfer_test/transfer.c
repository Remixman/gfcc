#include <stdio.h>
#include <math.h>
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
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			fprintf(stderr, "Error : CL_MISALIGNED_SUB_BUFFER_OFFSET\n"); break;
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

int main() {
	
	float *host_buffer = NULL;
	float *return_buffer = NULL;
	long long t0, t1, t2, t3;
	long long t4, t5, t6, t7;
	int i, j, correct = 1;
	
	cl_int status = CL_SUCCESS;
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	cl_mem cl_buffer, cl_subbuf;
	
	host_buffer = (float*) malloc(sizeof(float) * 1000000);
	for (i = 0; i < 1000000; i++)
		host_buffer[i] = (float)(i%1000);
	
	return_buffer = (float*) malloc(sizeof(float) * 250000);
	for (i = 0; i < 250000; i++)
		return_buffer[i] = 0.0;
	
	// initial GPU
	status = clGetPlatformIDs(1, &platform, NULL);
	_GfnCheckCLStatus(status, "clGetPlatformIDs");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	_GfnCheckCLStatus(status, "clGetDeviceIDs");	
	context = clCreateContext(0, 1, &device, NULL, NULL, &status);
	_GfnCheckCLStatus(status, "clCreateContext");
	queue = clCreateCommandQueue(context, device, 0, &status);
	_GfnCheckCLStatus(status, "clCreateCommandQueue");
	
	// create buffer
	t0 = get_time();
	cl_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * 1000000, NULL, &status);
	t1 = get_time();
	
	// send buffer
	t2 = get_time();
	status = clEnqueueWriteBuffer(queue, cl_buffer, CL_TRUE, 0, sizeof(float) * 1000000, host_buffer, 0, NULL, NULL);
	t3 = get_time();
	
	// create subbuffer
	t4 = get_time();
	cl_buffer_region info;
	info.origin = (size_t)(500000 * sizeof(float));
	info.size = (size_t)(250000 * sizeof(float));
	cl_subbuf = clCreateSubBuffer(cl_buffer, CL_MEM_READ_ONLY, CL_BUFFER_CREATE_TYPE_REGION, &info, &status);
	t5 = get_time();
	_GfnCheckCLStatus(status, "CREATE SUB BUFFER");
	
	t6 = get_time();
	status = clEnqueueWriteBuffer(queue, cl_subbuf, CL_TRUE, 0, sizeof(float) * 250000, host_buffer + 500000, 0, NULL, NULL);
	t7 = get_time();
	_GfnCheckCLStatus(status, "WRITE BUFFER");
	
	
	status = clEnqueueReadBuffer(queue, cl_subbuf, CL_TRUE, 0, sizeof(float) * 250000, return_buffer, 0, NULL, NULL);
	_GfnCheckCLStatus(status, "READ BUFFER");
	
	for (i = 0, j = 500000; i < 250000; i++, j++) {
		if (fabs((float)(j%1000) - return_buffer[i]) > 0.001) {
			correct = 0;
			break;
		}
	}

	if (correct) printf("PASS\n");
	else printf("FAIL\n");

	printf("Allocate time = %f msec.\n", (float)(t1-t0)/1000);
	printf("Transfer time = %f msec.\n", (float)(t3-t2)/1000);
	printf("Create subarray time = %f msec.\n", (float)(t5-t4)/1000);
	printf("Transfer subarray time = %f msec.\n", (float)(t7-t6)/1000);

	free(host_buffer);
	free(return_buffer);

	clReleaseMemObject(cl_buffer);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	return 0;
}

