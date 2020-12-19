/* KERNEL_DEFINITION _kernel_268428 */
const char *_kernel_268428_src = "__kernel void _kernel_268428(int gp0,int gp1,int gp2,int isize,int jsize,int ksize,__global float * us,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            us[((i) * ksize * jsize) + ((j) * ksize) + (k)] = i + j + k;""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 268428 _Function_268428 */
void _Function_268428()
{
    /* Declare Variables */
    int i;
    int gp0;
    int j;
    int gp1;
    int k;
    int gp2;
    int isize;
    int jsize;
    int ksize;
    float ***us;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_us;
    cl_mem _cl_mem_us = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&gp0, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp1, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp2, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&isize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&jsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_us, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc3D((void ****) &us, &_cl_mem_us, _id_us, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (isize) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (isize) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (gp0) - 1);
    _loop_size = _CalcLoopSize(0, (gp0) - 1, 1);
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
        _kernel = _GfnCreateKernel("_kernel_268428");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &gp0);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &gp1);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &gp2);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &isize);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &jsize);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &ksize);
        _GfnSetKernelArg(_kernel, 6, sizeof(cl_mem), (void *) &_cl_mem_us);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 8, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 9, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            i++ , _local_i += _loop_step)
        {
            for (j = 0;
                j < gp1;
                j++)
            {
                for (k = 0;
                    k < gp2;
                    k++)
                {
                    us[i][j][k] = i + j + k;
                }
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) us[0][0], _cl_mem_us, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_us, 1, 1);
}

/* KERNEL_DEFINITION _kernel_406940 */
const char *_kernel_406940_src = "__kernel void _kernel_406940(int gp0,int gp1,int gp2,int isize,int jsize,int ksize,__global float * lhs,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int m;""int n;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            for (m = 0;""                m < 5;""                m++)""            {""                for (n = 0;""                    n < 5;""                    n++)""                {""                    lhs[((0) * ksize * jsize) + ((m) * ksize) + (n)] = 0.0;""                    lhs[((1) * ksize * jsize) + ((m) * ksize) + (n)] = 0.0;""                    lhs[((2) * ksize * jsize) + ((m) * ksize) + (n)] = 0.0;""                }""            }""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 406940 _Function_406940 */
void _Function_406940()
{
    /* Declare Variables */
    int i;
    int gp0;
    int j;
    int gp1;
    int k;
    int gp2;
    int m;
    int n;
    int isize;
    int jsize;
    int ksize;
    float ******lhs;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_lhs;
    cl_mem _cl_mem_lhs = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&gp0, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp1, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp2, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&isize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&jsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_lhs, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc6D((void *******) &lhs, &_cl_mem_lhs, _id_lhs, _GFN_TYPE_FLOAT(), isize, jsize, ksize, 3, 5, 5, _GFN_MEM_WRITE_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (isize) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (isize) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (gp0) - 1);
    _loop_size = _CalcLoopSize(0, (gp0) - 1, 1);
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
        _kernel = _GfnCreateKernel("_kernel_406940");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &gp0);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &gp1);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &gp2);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &isize);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &jsize);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &ksize);
        _GfnSetKernelArg(_kernel, 6, sizeof(cl_mem), (void *) &_cl_mem_lhs);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 8, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 9, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            i++ , _local_i += _loop_