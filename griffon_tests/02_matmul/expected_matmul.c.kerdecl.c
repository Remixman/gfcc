/* KERNEL_DEFINITION _kernel_1 */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""__kernel void _kernel_1(int n,__global float * C,__global const float * A,__global const float * B,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (n) - 1) {\n""{""    for (j = 0;""        j < n;""        j++)""    {""        C[((i) * n) + (j)] = 0.f;""        for (k = 0;""            k < n;""            k++)""        {""            C[((i) * n) + (j)] += A[((i) * n) + (k)] * B[((k) * n) + (j)];""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 1 _Function_1 */
void _Function_1()
{
    /* Declare Variables */
    int i;
    int n;
    int j;
    int k;
    float **C;
    float **A;
    float **B;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
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
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&n, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_C, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &C, &_cl_mem_C, _id_C, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &A, &_cl_mem_A, _id_A, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc2D((void ***) &B, &_cl_mem_B, _id_B, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (n) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (n) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (n) - 1);
    _loop_size = _CalcLoopSize(0, (n) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) A[0], _cl_mem_A, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_ONLY(), 0, (n) - 1, 1, 0, 0, 1, 1, 2, 0, n, n);
    _GfnEnqueueBoardcastND((void *) B[0], _cl_mem_B, _GFN_TYPE_FLOAT(), 1, 1, 2, n, n);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_1");
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(int), (void *) &n);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_C);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(int), (void *) &_local_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 5, sizeof(int), (void *) &_local_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 6, sizeof(int), (void *) &_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            i++ , _local_i += _loop_step)
        {
            for (j = 0;
                j < n;
                j++)
            {
                C[i][j] = 0.f;
                for (k = 0;
                    k < n;
                    k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) C[0], _cl_mem_C, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (n) - 1, 1, 0, 0, 1, 1, 2, 0, n, n);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_C, 1, 1);
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_B, 1, 1);
}

