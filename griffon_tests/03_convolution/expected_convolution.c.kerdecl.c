/* KERNEL_DEFINITION _kernel_105999 */
const char *_kernel_105999_src = "__kernel void _kernel_105999(int Nsquare,int N,__global const double * matrix,__global double * result_matrix,int _local_tid_start,int _local_tid_end,int _loop_step) {\n""int i;int j;int m;int n;double new_val;""int _loop_size = (_local_tid_end - _local_tid_start) / _loop_step;\n""int _local_tid = get_global_id(0) * _loop_step;\n""int tid = _local_tid + _local_tid_start;\n""if (tid <= (Nsquare) - 1) {\n""{""    i = tid / N;""    j = tid % N;""    if (i >= 60 && i < N - 60 && j >= 60 && j < N - 60)""    {""        double new_val = 0.0;""        for (m = 0;""            m < 121;""            ++m)""        {""            for (n = 0;""                n < 121;""                ++n)""            {""                new_val += matrix[((i + m - 60) * N) + (j + n - 60)];""            }""        }""        result_matrix[((i) * N) + (j)] = new_val / 14641.0;""    }""    else""    {""        result_matrix[((i) * N) + (j)] = matrix[((i) * N) + (j)];""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 105999 _Function_105999 */
void _Function_105999()
{
    /* Declare Variables */
    int tid;
    int Nsquare;
    int i;
    int N;
    int j;
    int m;
    int n;
    double new_val;
    double **matrix;
    double **result_matrix;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_tid_start, _local_tid_end;
    int _loop_step;
    int _loop_size;
    int _local_tid = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    long long _id_result_matrix;
    cl_mem _cl_mem_result_matrix = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&Nsquare, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&N, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_result_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &result_matrix, &_cl_mem_result_matrix, _id_result_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (N) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (N) - 1);
    _local_tid_start = _GfnCalcLocalLoopStart2(0, (Nsquare) - 1, 1);
    _local_tid_end = _GfnCalcLocalLoopEnd2(0, (Nsquare) - 1, 1);
    _loop_size = _CalcLoopSize(0, (Nsquare) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) matrix[0], _cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), _GFN_MEM_READ_ONLY(), 0, (Nsquare) - 1, 1, 0, 1, 1, 1, 2, 2, N, N, - 60, 60);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_105999");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &Nsquare);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &N);
        _GfnSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_matrix);
        _GfnSetKernelArg(_kernel, 3, sizeof(cl_mem), (void *) &_cl_mem_result_matrix);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &_local_tid_start);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &_local_tid_end);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &_loop_step);
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
            if (i >= 60 && i < N - 60 && j >= 60 && j < N - 60)
            {
                double new_val = 0.0;
                for (m = 0;
                    m < 121;
                    ++m)
                {
                    for (n = 0;
                        n < 121;
                        ++n)
                    {
                        new_val += matrix[(i + m - 60)][(j + n - 60)];
                    }
                }
                result_matrix[i][j] = new_val / 14641.0;
            }
            else
            {
                result_matrix[i][j] = matrix[i][j];
            }
        }
    }
    /* Gather Array Memory */
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_matrix, 1, 1);
    _GfnFree(_id_result_matrix, 1, 1);
}

/* KERNEL_DEFINITION _kernel_896635 */
const char *_kernel_896635_src = "__kernel void _kernel_896635(int Nsquare,int N,__global double * matrix,__global double * result_matrix,int _local_tid_start,int _local_tid_end,int _loop_step) {\n""int i;""int j;""int _loop_size = (_local_tid_end - _local_tid_start) / _loop_step;\n""int _local_tid = get_global_id(0) * _loop_step;\n""int tid = _local_tid + _local_tid_start;\n""if (tid <= (Nsquare) - 1) {\n""{""    i = tid / N;""    j = tid % N;""    matrix[((i) * N) + (j)] = result_matrix[((i) * N) + (j)];""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 896635 _Function_896635 */
void _Function_896635()
{
    /* Declare Variables */
    int tid;
    int Nsquare;
    int i;
    int N;
    int j;
    double **matrix;
    double **result_matrix;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_tid_start, _local_tid_end;
    int _loop_step;
    int _loop_size;
    int _local_tid = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    long long _id_result_matrix;
    cl_mem _cl_mem_result_matrix = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&Nsquare, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&N, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_result_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    _GfnMalloc2D((void ***) &result_matrix, &_cl_mem_result_matrix, _id_result_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (N) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (N) - 1);
    _local_tid_start = _GfnCalcLocalLoopStart2(0, (Nsquare) - 1, 1);
    _local_tid_end = _GfnCalcLocalLoopEnd2(0, (Nsquare) - 1, 1);
    _loop_size = _CalcLoopSize(0, (Nsquare) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_896635");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &Nsquare);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &N);
        _GfnSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_matrix);
        _GfnSetKernelArg(_kernel, 3, sizeof(cl_mem), (void *) &_cl_mem_result_matrix);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &_local_tid_start);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &_local_tid_end);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &_loop_step);
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
            matrix[i][j] = result_matrix[i][j];
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) matrix[0], _cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), _GFN_MEM_READ_WRITE(), 0, (Nsquare) - 1, 1, 0, 0, 1, 1, 2, 0, N, N);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_matrix, 1, 1);
    _GfnFree(_id_result_matrix, 1, 1);
}

/* SEND_FUNCTION 511137 _Function_send_511137 */
void _Function_send_511137()
{
    /* Declare Variables */
    double **matrix;
    double **result_matrix;
    int N;
    /* Declare Generated Variables */
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    long long _id_result_matrix;
    cl_mem _cl_mem_result_matrix = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&_id_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_result_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&N, _GFN_TYPE_INT());
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    _GfnMalloc2D((void ***) &result_matrix, &_cl_mem_result_matrix, _id_result_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) matrix[0], _cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), _GFN_MEM_READ_WRITE(), 0, 0, 0, 0, 0, 1, 1, 2, 0, N, N);
    /* Lock Transfer */
    _GfnLockTransfer((long long) _id_matrix);
    _GfnLockTransfer((long long) _id_result_matrix);
    /* Deallocate Array Memory */
    _GfnFree(_id_matrix, 1, 1);
    _GfnFree(_id_result_matrix, 1, 1);
}

/* RECV_FUNCTION 471941 _Function_recv_471941 */
void _Function_recv_471941()
{
    /* Declare Variables */
    double **matrix;
    double **result_matrix;
    int N;
    /* Declare Generated Variables */
    long long _id_matrix;
    cl_mem _cl_mem_matrix = 0;
    long long _id_result_matrix;
    cl_mem _cl_mem_result_matrix = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&_id_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_result_matrix, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&N, _GFN_TYPE_INT());
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &matrix, &_cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    _GfnMalloc2D((void ***) &result_matrix, &_cl_mem_result_matrix, _id_result_matrix, _GFN_TYPE_DOUBLE(), N, N, _GFN_MEM_READ_WRITE(), 1, 1);
    /* Unlock Transfer */
    _GfnUnlockTransfer((long long) _id_matrix);
    _GfnUnlockTransfer((long long) _id_result_matrix);
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) matrix[0], _cl_mem_matrix, _id_matrix, _GFN_TYPE_DOUBLE(), _GFN_MEM_READ_WRITE(), 0, 0, 0, 0, 0, 1, 1, 2, 0, N, N);
    /* Deallocate Array Memory */
    _GfnFree(_id_matrix, 1, 1);
    _GfnFree(_id_result_matrix, 1, 1);
}

