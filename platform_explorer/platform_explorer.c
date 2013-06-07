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
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(0, 1, &device, NULL, NULL, &status);
	queue = clCreateCommandQueue(context, device, 0, &status);

if (crank == 0) {
	/* Test upload read only data */
	device_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, transfer_size, host_buffer, &status);
	time0 = get_time();
	status = clEnqueueWriteBuffer(queue, device_buffer, CL_TRUE, 0, transfer_size, host_buffer, 0, NULL, NULL);
	time1 = get_time();
	clReleaseMemObject(device_buffer);
	/* CL_MEM_WRITE_ONLY */
	printf("Time for upload read only data %zu bytes : %.10f\n", transfer_size, (float) (time1 - time0) / 1000000);
	
	/* Test upload read-write data */
	device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, transfer_size, host_buffer, &status);
	time0 = get_time();
	status = clEnqueueWriteBuffer(queue, device_buffer, CL_TRUE, 0, transfer_size, host_buffer, 0, NULL, NULL);
	time1 = get_time();
	clReleaseMemObject(device_buffer);
	/* CL_MEM_WRITE_ONLY */
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

