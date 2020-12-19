#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <gfn.h>

/* OpenCL Extension sources */
const char *cl_ext_src = 
	"#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
	"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n";

/* Function declaration sources */
const char *_decl_b_src = "int b(int x);\n";const char *_decl_c_src = "int c(int x);\n";const char *_decl_a_src = "int a(int x, int y);\n";
/* Function definition sources */
const char *_def_b_src = "int b(int x)""{""    return x * x;""}\n";
const char *_def_c_src = "int c(int x)""{""    return x + x + 1;""}\n";
const char *_def_a_src = "int a(int x, int y)""{""    return b(x) + c(y);""}\n";

/* Kernel sources */
const char *_kernel_474613_src = "__kernel void _kernel_474613(__global int * A,__global const int * B,int _local_i_start,int _local_i_end,int _loop_step) {\n""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (500) - 1) {\n""{""    A[(i)] = a(B[i], 8);""}\n""}\n""}\n""\n";

/* Worker function */
void _Function_474613()
{
    /* Declare Variables */
    int i;
    int *A;
    int *B;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc1D((void **) &A, &_cl_mem_A, _id_A, _GFN_TYPE_INT(), 500, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &B, &_cl_mem_B, _id_B, _GFN_TYPE_INT(), 500, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (500) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (500) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (500) - 1);
    _loop_size = _CalcLoopSize(0, (500) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) B, _cl_mem_B, _GFN_TYPE_INT(), _GFN_MEM_READ_ONLY(), 0, (500) - 1, 1, 0, 0, 1, 1, 1, 0, 500);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_474613");
        _GfnSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            i++ , _local_i += _loop_step)
        {
            A[i] = a(B[i], 8);
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) A, _cl_mem_A, _GFN_TYPE_INT(), _GFN_MEM_WRITE_ONLY(), 0, (500) - 1, 1, 0, 0, 1, 1, 1, 0, 500);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_B, 1, 1);
}
void _Function_send_775346()
{
    /* Declare Variables */
    int *A;
    int *B;
    /* Declare Generated Variables */
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    /* Allocate Array Memory */
    _GfnMalloc1D((void **) &A, &_cl_mem_A, _id_A, _GFN_TYPE_INT(), 500, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &B, &_cl_mem_B, _id_B, _GFN_TYPE_INT(), 500, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Distribute Array Memory */
    _GfnEnqueueBoardcastND((void *) B, _cl_mem_B, _GFN_TYPE_INT(), 1, 1, 1, 500);
    /* Lock Transfer */
    _GfnLockTransfer((void *) B);
    /* Deallocate Array Memory */
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_B, 1, 1);
}
void _Function_recv_532217()
{
    /* Declare Variables */
    int *A;
    int *B;
    /* Declare Generated Variables */
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    /* Allocate Array Memory */
    _GfnMalloc1D((void **) &A, &_cl_mem_A, _id_A, _GFN_TYPE_INT(), 500, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &B, &_cl_mem_B, _id_B, _GFN_TYPE_INT(), 500, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Unlock Transfer */
    _GfnUnlockTransfer((void *) A);
    /* Gather Array Memory */
    _SendOutputMsg((void *) A, (sizeof(int) * ((500))));
    /* Deallocate Array Memory */
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_B, 1, 1);
}

void RecvCommand(int *func_code) {
	if (_gfn_rank == 0) {
		_RecvCallFuncMsg(func_code);
		printf("Rank 0 receive command code : %d\n", *func_code);
	}
	MPI_Bcast( func_code, 1, MPI_INT, 0, MPI_COMM_WORLD );
}

int main(int argc, char *argv[]) {

	int exit_f = 0, error_f = 0;
	int func_code = 0;
	char * total_src = NULL;

	_GfnInit(&argc, &argv);
	total_src = (char*) malloc(847);

	total_src[0] = '\0';
	strcat(total_src, cl_ext_src);
	strcat(total_src, _decl_b_src);
	strcat(total_src, _decl_c_src);
	strcat(total_src, _decl_a_src);
	strcat(total_src, _def_b_src);
	strcat(total_src, _def_c_src);
	strcat(total_src, _def_a_src);
	strcat(total_src, _kernel_474613_src);

	_GfnCreateProgram(total_src);

	while (1) {
		RecvCommand(&func_code);

		switch (func_code) {
		case 474613: _Function_474613(); break;
		case 775346: _Function_send_775346(); break;
		case 532217: _Function_recv_532217(); break;

		case 0:
			if (_gfn_rank == 0) {
				printf("Get exit code!\n");
			}
			exit_f = 1;
			break;

		case -1:
			exit_f = error_f = 1;
			break;

		default:
			// TODO: show error unknown command
			if (_gfn_rank == 0) {
				printf("Get Error Code : %d\n", func_code);
			}
			exit_f = 1;
			break;
		}

		if (exit_f) break;
	}

	free(total_src);
	_GfnClearProgram();
	_GfnFinalize();

	if (error_f)
		printf("Exit with error");

	return 0;
} /* end main */

