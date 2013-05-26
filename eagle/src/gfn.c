#include <math.h>
#include <stdio.h>
#include "gfn.h"


// API for user
int gfn_get_num_process()
{
	return _gfn_num_proc;
}

int gfn_get_process_num()
{
	return _gfn_rank;
}


// Higher Level function
void _GfnBarrier()
{
	MPI_Barrier(MPI_COMM_WORLD);
}

void _GfnCheckCLStatus(cl_int status, const char *phase_name)
{
	if (status != CL_SUCCESS)
	{
		fprintf(stderr, "**[OpenCL] Rank #%d Runtime error in %s phase**\n", 
			_gfn_rank, phase_name);

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

size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num)
{
	size_t div_group_num = ((work_item_num + work_group_item_num - 1) / work_group_item_num);
	return (work_group_item_num * div_group_num);
}


int _gfn_rank;		/**/
int _gfn_num_proc;	/**/


int _CalcLoopSize(int start, int end, int incre)
{
	return ceil((float)(end - start + 1) / (float)incre);
}

int _CalcSubSize(int size, int num_proc, int rank, int block_size)
{
	int result = 0;
	if (rank < 0) return 0; // for start index of rank 0
	result = (size/num_proc) + (int)(rank < (size%num_proc));
	result *= block_size;
	return result;
}

int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank)
{
	int range_size = old_end - old_start + 1;
	return old_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank);
}

int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank)
{
	int range_size = old_end - old_start + 1;
	return old_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank + 1);
}

int _CalcOffset(int size, int num_proc, int rank)
{
	int offset = (size/num_proc)*(rank);
	if (rank <= size%num_proc)
		offset += rank;
	else
		offset += size%num_proc;
	return offset;
}

void _CalcCnts(int size, int num_proc, int *cnts /* OUTS */, int block_size)
{
	int i;
	for (i = 0; i < num_proc; ++i) {
		cnts[i] = _CalcSubSize(size, num_proc, i, 1) * block_size;
#ifdef API_DEBUG
		printf("Rank #%d , count[%d] : %d\n", _gfn_rank, i, cnts[i]);
#endif // API_DEBUG
	}
}

void _CalcDisp(int size, int num_proc, int *disp /* OUTS */, int block_size)
{
	int i;
	for (i = 0; i < num_proc; ++i) {
		disp[i] = _CalcOffset(size, num_proc, i) * block_size;
#ifdef API_DEBUG
		printf("Rank #%d , disp[%d] : %d\n", _gfn_rank, i, disp[i]);
#endif // API_DEBUG
	}
}


MPI_Datatype _get_mpi_char() 			{ return MPI_CHAR; }
MPI_Datatype _get_mpi_unsigned_char()	{ return MPI_UNSIGNED_CHAR; }
MPI_Datatype _get_mpi_byte() 			{ return MPI_BYTE; }
MPI_Datatype _get_mpi_short() 			{ return MPI_SHORT; }
MPI_Datatype _get_mpi_unsigned_short() 	{ return MPI_UNSIGNED_SHORT; }
MPI_Datatype _get_mpi_int() 			{ return MPI_INT; }
MPI_Datatype _get_mpi_unsigned() 		{ return MPI_UNSIGNED; }
MPI_Datatype _get_mpi_long() 			{ return MPI_LONG; }
MPI_Datatype _get_mpi_unsigned_long() 	{ return MPI_UNSIGNED_LONG; }
MPI_Datatype _get_mpi_float() 			{ return MPI_FLOAT; }
MPI_Datatype _get_mpi_double() 			{ return MPI_DOUBLE; }
MPI_Datatype _get_mpi_long_double() 	{ return MPI_LONG_DOUBLE; }
MPI_Datatype _get_mpi_long_long_int() 	{ return MPI_LONG_LONG_INT; }

MPI_Op _get_mpi_max() 		{ return MPI_MAX; }
MPI_Op _get_mpi_min() 		{ return MPI_MIN; }
MPI_Op _get_mpi_sum() 		{ return MPI_SUM; }
MPI_Op _get_mpi_prod() 		{ return MPI_PROD; }
MPI_Op _get_mpi_land() 		{ return MPI_LAND; }
MPI_Op _get_mpi_band() 		{ return MPI_BAND; }
MPI_Op _get_mpi_lor() 		{ return MPI_LOR; }
MPI_Op _get_mpi_bor() 		{ return MPI_BOR; }
MPI_Op _get_mpi_lxor() 		{ return MPI_LXOR; }
MPI_Op _get_mpi_bxor() 		{ return MPI_BXOR; }
MPI_Op _get_mpi_minloc() 	{ return MPI_MINLOC; }
MPI_Op _get_mpi_maxloc() 	{ return MPI_MAXLOC; }

MPI_Comm _get_mpi_comm_world() 			{ return MPI_COMM_WORLD; }


// Function for OpenCL
void _InitOpenCL()
{
	cl_uint /*num_platforms, */num_devices;
	
	_gfn_status = clGetPlatformIDs(1, &_gfn_platform_id, NULL/*&num_platforms*/);
	_GfnCheckCLStatus(_gfn_status, "clGetPlatformIDs");

	_gfn_status = clGetDeviceIDs(_gfn_platform_id, CL_DEVICE_TYPE_DEFAULT/*CL_DEVICE_TYPE_GPU*/, 1,
		&_gfn_device_id, &num_devices);
	_GfnCheckCLStatus(_gfn_status, "clGetDeviceIDs");

	_gfn_context = clCreateContext(NULL, 1, &_gfn_device_id,
		NULL, NULL, &_gfn_status);
	_GfnCheckCLStatus(_gfn_status, "clCreateContext");

	_gfn_cmd_queue = clCreateCommandQueue(_gfn_context, _gfn_device_id, 0, &_gfn_status);
	_GfnCheckCLStatus(_gfn_status, "clCreateCommandQueue");
}

cl_platform_id _gfn_platform_id;
cl_device_id _gfn_device_id;
cl_context _gfn_context;
cl_command_queue _gfn_cmd_queue;
cl_int _gfn_status;

void _FinalOpenCL()
{
	// Block until all commands in queue have completed
	_gfn_status = clFlush(_gfn_cmd_queue);
	_GfnCheckCLStatus(_gfn_status, "clFlush");

	// Block until all commands in queue have been removed from the queue
    _gfn_status = clFinish(_gfn_cmd_queue);
    _GfnCheckCLStatus(_gfn_status, "clFinish");

	_gfn_status = clReleaseContext(_gfn_context);
	_GfnCheckCLStatus(_gfn_status, "clReleaseContext");

    //clReleaseKernel(kernel);
    //clReleaseProgram(program);
}

cl_kernel _CreateKernelFromSource(const char *name, const char *src, 
                                  cl_context context, cl_device_id device_id)
{
	cl_int status;
	cl_kernel kernel = NULL;
	size_t param_value_size = 1024 * 1024;
    size_t param_value_size_ret;
    char param_value[param_value_size];

    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&src, NULL, &status);
    _GfnCheckCLStatus(status, "CREATE PROGRAM WITH SOURCE");
    status = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    _GfnCheckCLStatus(status, "BUILD PROGRAM");

    /* Get kernel compiler messages */
    status = clGetProgramBuildInfo(program,
                                   device_id,
                                   CL_PROGRAM_BUILD_LOG,
                                   param_value_size,
                                   param_value,
                                   &param_value_size_ret);

    // TODO: why length 2 for success build
    if (param_value_size_ret != 2) {
        printf("Message from kernel compiler : \n%s\n", param_value);
    }

    kernel = clCreateKernel(program, name, &status);
    _GfnCheckCLStatus(status, "CREATE KERNEL");
    
    return kernel;
}

cl_bool _get_cl_true() 		{ return CL_TRUE; }
cl_bool _get_cl_false() 	{ return CL_FALSE; }

cl_mem_flags _get_cl_mem_read_write() 		{ return CL_MEM_READ_WRITE; }
cl_mem_flags _get_cl_mem_write_only() 		{ return CL_MEM_WRITE_ONLY; }
cl_mem_flags _get_cl_mem_read_only() 		{ return CL_MEM_READ_ONLY; }
cl_mem_flags _get_cl_mem_use_host_ptr() 	{ return CL_MEM_USE_HOST_PTR; }
cl_mem_flags _get_cl_mem_alloc_host_ptr() 	{ return CL_MEM_ALLOC_HOST_PTR; }

