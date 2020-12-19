#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <gfn.h>

/* OpenCL Extension sources */
const char *cl_ext_src = 
	"#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
	"#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n";

/* Function declaration sources */

/* Function definition sources */

/* Kernel sources */
const char *_kernel_269511_src = "__kernel void _kernel_269511(int n,__global double * vec2,__global const double * vec1,int _local_i_start,int _local_i_end,int _loop_step) {\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (n) - 1) {\n""{""    if (i > 0 && i < n - 1)""        vec2[(i)] = (vec1[(i - 1)] + vec1[(i)] + vec1[(i + 1)]) / 3.0;""    else""        vec2[(i)] = vec1[(i)];""}\n""}\n""}\n""\n";

/* Worker function */
void _Function_269511()
{
    /* Declare Variables */
    int i;
    int n;
    double *vec2;
    double *vec1;
    /* Declare Generated Variables */
    int _kernel_id = 269511;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    struct _kernel_information *_kernel_info = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_vec2;
    cl_mem _cl_mem_vec2 = 0;
    long long _id_vec1;
    cl_mem _cl_mem_vec1 = 0;
    /* Broadcast Scalar Value */
    _GfnEnqueueBroadcastScalar(&n, _GFN_TYPE_INT());
    _GfnEnqueueBroadcastScalar(&_id_vec2, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBroadcastScalar(&_id_vec1, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBroadcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc1D((void **) &vec2, &_cl_mem_vec2, _id_vec2, _GFN_TYPE_DOUBLE(), n, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &vec1, &_cl_mem_vec1, _id_vec1, _GFN_TYPE_DOUBLE(), n, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_i_start = _GfnCalcLocalLoopStart(0, (n) - 1, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(0, (n) - 1, 1);
    _loop_size = _CalcLoopSize(0, (n) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) vec1, _cl_mem_vec1, _id_vec1, _GFN_TYPE_DOUBLE(), _GFN_MEM_READ_ONLY(), 0, (n) - 1, 1, 0, 1, 1, 1, 1, 2, n, - 1, 1);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_269511");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &n);
        _GfnSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_vec2);
        _GfnSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_vec1);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num, 0);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            ++i , _local_i += _loop_step)
        {
            if (i > 0 && i < n - 1)
                vec2[i] = (vec1[i - 1] + vec1[i] + vec1[i + 1]) / 3.0;
            else
                vec2[i] = vec1[i];
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) vec2, _cl_mem_vec2, _id_vec2, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (n) - 1, 1, 0, 0, 1, 1, 1, 0, n);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_vec2, 1, 1);
    _GfnFree(_id_vec1, 1, 1);
}

void RecvCommand(int *func_code) {
	if (_gfn_rank == 0) {
		_RecvCallFuncMsg(func_code);
	}
	MPI_Bcast( func_code, 1, MPI_INT, 0, MPI_COMM_WORLD );
}

int main(int argc, char *argv[]) {

	int exit_f = 0, error_f = 0;
	int func_code = 0;
	char * total_src = NULL;

	_GfnInit(&argc, &argv);
	total_src = (char*) malloc(554);

	total_src[0] = '\0';
	strcat(total_src, cl_ext_src);
	strcat(total_src, _kernel_269511_src);

	_GfnCreateProgram(total_src);

	while (1) {
		RecvCommand(&func_code);

		switch (func_code) {
		case 269511: _Function_269511(); break;

		case 0:
			if (_gfn_rank == 0) {
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

