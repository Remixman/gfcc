/* KERNEL_DEFINITION _kernel_768447 */
const char *_kernel_768447_src = "__kernel void _kernel_768447(int nsquare,int n,int i,int j,__global const float * B,__global const float * A,__global float * C,int _local_tid_start,int _local_tid_end,int _loop_step) {\n""int k;""float tmp;""int _loop_size = (_local_tid_end - _local_tid_start) / _loop_step;\n""int _local_tid = get_global_id(0) * _loop_step;\n""int tid = _local_tid + _local_tid_start;\n""if (tid <= (nsquare) - 1) {\n""{""    int i = tid / n;""    int j = tid % n;""    int k = 0;""    float tmp = 0.0;""    for (k = 0;""        k < n;""        ++k)""        tmp += A[((i) * n) + (k)] * B[((k) * n) + (j)];""    C[((i) * n) + (j)] = tmp;""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 768447 _Function_768447 */
void _Function_768447()
{
    /* Declare Variables */
    int tid;
    int nsquare;
    int n;
    int k;
    float tmp;
    int i;
    int j;
    float **B;
    float **A;
    float **C;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_tid_start, _local_tid_end;
    int _loop_step;
    int _loop_size;
    int _local_tid = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_C;
    cl_mem _cl_mem_C = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&nsquare, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&n, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&i, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&j, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_C, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &B, &_cl_mem_B, _id_B, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &A, &_cl_mem_A, _id_A, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &C, &_cl_mem_C, _id_C, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (n) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (n) - 1);
    _local_tid_start = _GfnCalcLocalLoopStart2(0, (nsquare) - 1, 1);
    _local_tid_end = _GfnCalcLocalLoopEnd2(0, (nsquare) - 1, 1);
    _loop_size = _CalcLoopSize(0, (nsquare) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueBoardcastND((void *) B[0], _cl_mem_B, _id_B, _GFN_TYPE_FLOAT(), 1, 1, 2, n, n);
    _GfnEnqueueScatterND((void *) A[0], _cl_mem_A, _id_A, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_ONLY(), 0, (nsquare) - 1, 1, 0, 0, 1, 1, 2, 0, n, n);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_768447");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &nsquare);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &n);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &i);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &j);
        _GfnSetKernelArg(_kernel, 4, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnSetKernelArg(_kernel, 5, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnSetKernelArg(_kernel, 6, sizeof(cl_mem), (void *) &_cl_mem_C);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &_local_tid_start);
        _GfnSetKernelArg(_kernel, 8, sizeof(int), (void *) &_local_tid_end);
        _GfnSetKernelArg(_kernel, 9, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (tid = _local_tid_start;
            tid <= _local_tid_end;
            ++tid , _local_tid += _loop_step)
        {
            int i = tid / n;
            int j = tid % n;
            int k = 0;
            float tmp = 0.0;
            for (k = 0;
                k < n;
                ++k)
                tmp += A[i][k] * B[k][j];
            C[i][j] = tmp;
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) C[0], _cl_mem_C, _id_C, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 0, (nsquare) - 1, 1, 0, 0, 1, 1, 2, 0, n, n);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_B, 1, 1);
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_C, 1, 1);
}

