/* KERNEL_DEFINITION _kernel_1 */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""__kernel void _kernel_1(int n,__global int * _global_reduction_s1,__local int * _cl_local_mem_s1,__global float * _global_reduction_s2,__local float * _cl_local_mem_s2,int _local_i_start,int _local_i_end,int _loop_step) {\n""int n;""int s1;""float s2;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""s1 = 0;\n""s2 = 0;\n""if (i <= n) {\n""{""    s1 += i;""    s2 += (float) i;""}\n""}\n""if (get_global_id(0) < _loop_size) {\n""_cl_local_mem_s1[get_local_id(0)] = s1;\n""_cl_local_mem_s2[get_local_id(0)] = s2;\n""} else {\n""_cl_local_mem_s1[get_local_id(0)] = 0;\n""_cl_local_mem_s2[get_local_id(0)] = 0;\n""}\n""for (int _stride = get_local_size(0)/2; _stride > 0; _stride /= 2) {\n""barrier(CLK_LOCAL_MEM_FENCE);\n""if (get_local_id(0) < _stride) {\n""_cl_local_mem_s1[get_local_id(0)] += _cl_local_mem_s1[get_local_id(0)+_stride];\n""_cl_local_mem_s2[get_local_id(0)] += _cl_local_mem_s2[get_local_id(0)+_stride];\n""}\n""}\n""barrier(CLK_GLOBAL_MEM_FENCE);\n""if (get_local_id(0) == 0) {\n""_global_reduction_s1[get_group_id(0)] = _cl_local_mem_s1[0];\n""_global_reduction_s2[get_group_id(0)] = _cl_local_mem_s2[0];\n""}\n""}\n""\n";

/* WORKER_FUNCTION 1 _Function_1 */
void _Function_1()
{
    /* Declare Variables */
    int i;
    int n;
    int s1;
    float s2;
    /* Declare Generated Variables */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    cl_mem _cl_mem_s1 = 0;
    cl_mem _cl_mem_s2 = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&n, _GFN_TYPE_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    /* Initialize Generated Variables */
    _local_i_start = _CalcLocalStartIndex(1, n, _gfn_num_proc, _gfn_rank + 1);
    _local_i_end = _CalcLocalEndIndex(1, n, _gfn_num_proc, _gfn_rank + 1);
    _loop_size = _CalcLoopSize(1, n, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    _GfnMallocReduceScalar(&s1, &_cl_mem_s1, _GFN_TYPE_INT(), _global_item_num / _work_group_item_num, 1, 1);
    _GfnMallocReduceScalar(&s2, &_cl_mem_s2, _GFN_TYPE_FLOAT(), _global_item_num / _work_group_item_num, 1, 1);
    /* Distribute Array Memory */
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_1");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &n);
        _GfnSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_s1);
        _GfnSetKernelArg(_kernel, 2, sizeof(int) * _work_group_item_num, 0);
        _GfnSetKernelArg(_kernel, 3, sizeof(cl_mem), (void *) &_cl_mem_s2);
        _GfnSetKernelArg(_kernel, 4, sizeof(float) * _work_group_item_num, 0);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            i++ , _local_i += _loop_step)
        {
            s1 += i;
            s2 += (float) i;
        }
    }
    /* Gather Array Memory */
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnEnqueueReduceScalar(&s1, _cl_mem_s1, _GFN_TYPE_INT(), _GFN_OP_SUM(), _global_item_num / _work_group_item_num, 1, 1);
    _GfnEnqueueReduceScalar(&s2, _cl_mem_s2, _GFN_TYPE_FLOAT(), _GFN_OP_SUM(), _global_item_num / _work_group_item_num, 1, 1);
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFreeReduceScalar(_cl_mem_s1, 1, 1);
    _GfnFreeReduceScalar(_cl_mem_s2, 1, 1);
}

