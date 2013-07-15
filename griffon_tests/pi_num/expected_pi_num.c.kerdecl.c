/* KERNEL_DEFINITION _kernel_1 */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""__kernel void _kernel_1(int n,double h,__global double * _global_reduction_sum,__local double * _cl_local_mem_sum,int _local_i_start,int _local_i_end,int _loop_step) {\n""double x;""double sum;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""sum = 0;\n""if (i <= n) {\n""{""    x = h * ((double) i - 0.5);""    sum += 4.0 / (1.0 + x * x);""}\n""}\n""if (get_global_id(0) < _loop_size) {\n""_cl_local_mem_sum[get_local_id(0)] = sum;\n""} else {\n""_cl_local_mem_sum[get_local_id(0)] = 0;\n""}\n""for (int _stride = get_local_size(0)/2; _stride > 0; _stride /= 2) {\n""barrier(CLK_LOCAL_MEM_FENCE);\n""if (get_local_id(0) < _stride) {\n""_cl_local_mem_sum[get_local_id(0)] += _cl_local_mem_sum[get_local_id(0)+_stride];\n""}\n""}\n""barrier(CLK_GLOBAL_MEM_FENCE);\n""if (get_local_id(0) == 0) {\n""_global_reduction_sum[get_group_id(0)] = _cl_local_mem_sum[0];\n""}\n""}\n""\n";

/* WORKER_FUNCTION 1 _Function_1 */
void _Function_1()
{
    /* Declare Variables */
    int i;
    int n;
    double x;
    double h;
    double sum;
    /* Declare Generated Variables */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    cl_mem _cl_mem_sum = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&n, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&h, _GFN_TYPE_DOUBLE());
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
    _GfnMallocReduceScalar(&sum, &_cl_mem_sum, _GFN_TYPE_DOUBLE(), _global_item_num / _work_group_item_num, 1, 1);
    /* Distribute Array Memory */
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_1");
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(int), (void *) &n);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG 0");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(double), (void *) &h);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG 1");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_sum);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG 2");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(double) * _work_group_item_num, 0);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG (LOCAL SIZE)");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(int), (void *) &_local_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG 4");
        _gfn_status = clSetKernelArg(_kernel, 5, sizeof(int), (void *) &_local_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG 5");
        _gfn_status = clSetKernelArg(_kernel, 6, sizeof(int), (void *) &_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG 6");
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
            x = h * ((double) i - 0.5);
            sum += 4.0 / (1.0 + x * x);
        }
    }
    /* Gather Array Memory */
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnEnqueueReduceScalar(&sum, _cl_mem_sum, _GFN_TYPE_DOUBLE(), _GFN_OP_SUM(), _global_item_num / _work_group_item_num, 1, 1);
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFreeReduceScalar(_cl_mem_sum, 1, 1);
}

