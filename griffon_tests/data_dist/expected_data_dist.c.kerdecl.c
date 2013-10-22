/* KERNEL_DEFINITION _kernel_793671 */
const char *_kernel_793671_src = "__kernel void _kernel_793671(__global const int * B,__global int * A,__global const int * C,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (1000) - 1) {\n""{""    A[(i)] = B[(i)];""    for (j = 1;""        j < 4;""        ++j)""    {""        A[(i)] += C[(j)] + C[(j + 3)];""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 793671 _Function_793671 */
void _Function_793671()
{
    /* Declare Variables */
    int i;
    int j;
    int *B;
    int *A;
    int *C;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_C;
    cl_mem _cl_mem_C = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_C, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc1D((void **) &B, &_cl_mem_B, _id_B, _GFN_TYPE_INT(), 1000, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &A, &_cl_mem_A, _id_A, _GFN_TYPE_INT(), 1000, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &C, &_cl_mem_C, _id_C, _GFN_TYPE_INT(), 7, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (1000) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (1000) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (1000) - 1);
    _loop_size = _CalcLoopSize(0, (1000) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) B, _cl_mem_B, _GFN_TYPE_INT(), _GFN_MEM_READ_ONLY(), 0, (1000) - 1, 1, 0, 0, 1, 1, 1, 0, 1000);
    _GfnEnqueueBoardcastND((void *) C, _cl_mem_C, _GFN_TYPE_INT(), 1, 1, 1, 7);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_793671");
        _GfnSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_C);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            ++i , _local_i += _loop_step)
        {
            A[i] = B[i];
            for (j = 1;
                j < 4;
                ++j)
            {
                A[i] += C[j] + C[j + 3];
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) A, _cl_mem_A, _GFN_TYPE_INT(), _GFN_MEM_WRITE_ONLY(), 0, (1000) - 1, 1, 0, 0, 1, 1, 1, 0, 1000);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_B, 1, 1);
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_C, 1, 1);
}

