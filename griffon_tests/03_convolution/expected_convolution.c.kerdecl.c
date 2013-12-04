/* KERNEL_DEFINITION _kernel_291464 */
const char *_kernel_291464_src = "__kernel void _kernel_291464(int Nsquare,int N,int filterN,__global const float * filter,__global float * matrix,int _local_tid_start,int _local_tid_end,int _loop_step) {\n""int i;int j;int m;int n;float new_val;""int _loop_size = (_local_tid_end - _local_tid_start) / _loop_step;\n""int _local_tid = get_global_id(0) * _loop_step;\n""int tid = _local_tid + _local_tid_start;\n""if (tid <= (Nsquare) - 1) {\n""{""    i = tid / N;""    j = tid % N;""    if (i >= 2 && i < N - 2 && j >= 2 && j < N - 2)""    {""        float new_val = 0.0;""        for (m = 0;""            m < filterN;""            ++m)""        {""            for (n = 0;""                n < filterN;""                ++n)""            {""                new_val += (filter[((m) * 5) + (n)] * matrix[((i + m - 2) * N) + (j + n - 2)]);""            }""        }""        matrix[((i) * N) + (j)] = new_val;""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 291464 _Function_291464 */
void _Function_291464()
{
    /* Declare Variables */
    int tid;
    int Nsquare;
    int i;
    int N;
    int j;
    int m;
    int filterN;
    int n;
    float new_val;
    float **filter;
    float **matrix;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_tid_start, _local_tid_end;
    int _loop_step;
    int _loop_size;
    int _local_tid = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_filter;
    cl_mem _cl_mem_filter = 0;
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&Nsquare, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&N, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&filterN, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_filter, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &filter, &_cl_mem_filter, _id_filter, _GFN_TYPE_FLOAT(), 5, 5, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_FLOAT(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (N) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (N) - 1);
    _local_tid_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_tid_end = _GfnCalcLocalLoopEnd(_local_data_end, (Nsquare) - 1);
    _loop_size = _CalcLoopSize(0, (Nsquare) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueBoardcastND((void *) filter[0], _cl_mem_filter, _GFN_TYPE_FLOAT(), 1, 1, 2, 5, 5);
    _GfnEnqueueScatterND((void *) matrix[0], _cl_mem_matrix, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 0, (Nsquare) - 1, 1, 0, 1, 1, 1, 2, 2, N, N, - 2, 2);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_291464");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &Nsquare);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &N);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &filterN);
        _GfnSetKernelArg(_kernel, 3, sizeof(cl_mem), (void *) &_cl_mem_filter);
        _GfnSetKernelArg(_kernel, 4, sizeof(cl_mem), (void *) &_cl_mem_matrix);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &_local_tid_start);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &_local_tid_end);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (tid = _local_tid_start;
            tid <= _local_tid_end;
            ++tid , _local_tid += _loop_step)
        {
            i = tid / N;
            j = tid % N;
            if (i >= 2 && i < N - 2 && j >= 2 && j < N - 2)
            {
                float new_val = 0.0;
                for (m = 0;
                    m < filterN;
                    ++m)
                {
                    for (n = 0;
                        n < filterN;
                        ++n)
                    {
                        new_val += (filter[m][n] * matrix[(i + m - 2)][(j + n - 2)]);
                    }
                }
                matrix[i][j] = new_val;
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) matrix[0], _cl_mem_matrix, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 0, (Nsquare) - 1, 1, 0, 0, 1, 1, 2, 0, N, N);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_filter, 1, 1);
    _GfnFree(_id_matrix, 1, 1);
}

/* SEND_FUNCTION 631189 _Function_send_631189 */
void _Function_send_631189()
{
    /* Declare Variables */
    float **filter;
    float **matrix;
    int N;
    /* Declare Generated Variables */
    long long _id_filter;
    cl_mem _cl_mem_filter = 0;
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &filter, &_cl_mem_filter, _id_filter, _GFN_TYPE_FLOAT(), 5, 5, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_FLOAT(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Distribute Array Memory */
    _GfnEnqueueBoardcastND((void *) filter[0], _cl_mem_filter, _GFN_TYPE_FLOAT(), 1, 1, 2, 5, 5);
    _GfnEnqueueScatterND((void *) matrix[0], _cl_mem_matrix, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 0, 0, 0, 0, 0, 1, 1, 2, 0, N, N);
    _GfnEnqueueBoardcastScalar(&N, _GFN_TYPE_INT());
    /* Lock Transfer */
    _GfnLockTransfer((void *) filter[0]);
    _GfnLockTransfer((void *) matrix[0]);
    /* Deallocate Array Memory */
    _GfnFree(_id_filter, 1, 1);
    _GfnFree(_id_matrix, 1, 1);
}

/* RECV_FUNCTION 710778 _Function_recv_710778 */
void _Function_recv_710778()
{
    /* Declare Variables */
    float **filter;
    float **matrix;
    int N;
    /* Declare Generated Variables */
    long long _id_filter;
    cl_mem _cl_mem_filter = 0;
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &filter, &_cl_mem_filter, _id_filter, _GFN_TYPE_FLOAT(), 5, 5, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_FLOAT(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Unlock Transfer */
    _GfnUnlockTransfer((void *) matrix[0]);
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) matrix[0], _cl_mem_matrix, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 0, 0, 0, 0, 0, 1, 1, 2, 0, N, N);
    /* Deallocate Array Memory */
    _GfnFree(_id_filter, 1, 1);
    _GfnFree(_id_matrix, 1, 1);
}

