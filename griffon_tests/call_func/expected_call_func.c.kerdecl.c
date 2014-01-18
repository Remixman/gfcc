/* KERNEL_DEFINITION _kernel_863273 */
const char *_kernel_863273_src = "__kernel void _kernel_863273(__global int * C,__global const int * A,__global const int * B,int _local_i_start,int _local_i_end,int _loop_step) {\n"
"int j;\n"
"int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n"
"int _local_i = get_global_id(0) * _loop_step;\n"
"int i = _local_i + _local_i_start;\n"
"if (i <= (50) - 1) {\n"
"{""    for (j = 0;""        j < 50;""        ++j)""    {""        C[((i) * 50) + (j)] = A[((i) * 50) + (j)] + B[((i) * 50) + (j)];""    }""}\n"
"}\n"
"}\n"
"\n";

/* WORKER_FUNCTION 863273 _Function_863273 */
void _Function_863273()
{
    /* Declare Variables */
    int i;
    int j;
    int **C;
    int **A;
    int **B;
    /* Declare Generated Variables */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_C;
    cl_mem _cl_mem_C = 0;
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    /* Broadcast Scalar Value */
    _GfnEnqueueBroadcastScalar(&_id_C, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBroadcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBroadcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBroadcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &C, &_cl_mem_C, _id_C, _GFN_TYPE_INT(), 50, 50, _GFN_MEM_READ_WRITE(), 1, 1);
    _GfnMalloc2D((void ***) &A, &_cl_mem_A, _id_A, _GFN_TYPE_INT(), 50, 50, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &B, &_cl_mem_B, _id_B, _GFN_TYPE_INT(), 50, 50, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_i_start = _GfnCalcLocalLoopStart2(0, (50) - 1, 1);
    _local_i_end = _GfnCalcLocalLoopEnd2(0, (50) - 1, 1);
    _loop_size = _CalcLoopSize(0, (50) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) A[0], _cl_mem_A, _id_A, _GFN_TYPE_INT(), _GFN_MEM_READ_ONLY(), 0, (50) - 1, 1, 0, 0, 1, 1, 2, 0, 50, 50);
    _GfnEnqueueBroadcastND((void *) B[0], _cl_mem_B, _id_B, _GFN_TYPE_INT(), 1, 1, 2, 50, 50);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_863273");
        _GfnSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_C);
        _GfnSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_B);
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
            for (j = 0;
                j < 50;
                ++j)
            {
                C[i][j] = A[i][j] + B[i][j];
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) C[0], _cl_mem_C, _id_C, _GFN_TYPE_INT(), _GFN_MEM_READ_WRITE(), 0, (50) - 1, 1, 0, 0, 1, 1, 2, 0, 50, 50);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_C, 1, 1);
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_B, 1, 1);
}

