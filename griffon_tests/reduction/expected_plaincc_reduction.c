/* */ #ifdef GFN_WORKER /* */
/* #pragma OPENCL EXTENSION cl_khr_fp64 : enable\n */
/* #pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n */
/* void _GfnBarrier() {\n */
/*     barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n */
/* }\n */
/* int _GfnAtomicAddInt(__global int* const address, const int value) {\n */
/*     return atomic_add(address, value);\n */
/* }\n */
/* float _GfnAtomicAddFloat(__global float* const address, const float value) {\n */
/*     uint oldval, newval, readback;\n */
/*     *(float*)&oldval = *address;\n */
/*     *(float*)&newval = (*(float*)&oldval + value);\n */
/*     while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {\n */
/*         oldval = readback;\n */
/*         *(float*)&newval = (*(float*)&oldval + value);\n */
/*     }\n */
/*     return *(float*)&oldval;\n */
/* }\n */
/* double _GfnAtomicAddDouble(__global double* const address, const double value) {\n */
/*     long oldval, newval, readback;\n */
/*     *(double*)&oldval = *address;\n */
/*     *(double*)&newval = (*(double*)&oldval + value);\n */
/*     while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {\n */
/*         oldval = readback;\n */
/*         *(double*)&newval = (*(double*)&oldval + value);\n */
/*     }\n */
/*     return *(double*)&oldval;\n */
/* }\n */
/* __kernel void _kernel_1(__global int * _global_reduction_s1,__local int * _cl_local_mem_s1,__global float * _global_reduction_s2,__local float * _cl_local_mem_s2,int _local_i_start,int _local_i_end,int _loop_step) {\n */
/* int n; */
/* int s1 = 0;\n */
/* float s2 = 0;\n */
/* int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n */
/* int _local_i = get_global_id(0) * _loop_step;\n */
/* int i = _local_i + _local_i_start;\n */
/* if (i <= n) {\n */
/* { */
/*     s1 += i; */
/*     s2 += (float) i; */
/* }\n */
/* }\n */
/* if (get_global_id(0) < _loop_size) {\n */
/* _cl_local_mem_s1[get_local_id(0)] = s1;\n */
/* _cl_local_mem_s2[get_local_id(0)] = s2;\n */
/* } else {\n */
/* _cl_local_mem_s1[get_local_id(0)] = 0;\n */
/* _cl_local_mem_s2[get_local_id(0)] = 0;\n */
/* }\n */
/* for (int _stride = get_local_size(0)/2; _stride > 0; _stride /= 2) {\n */
/* barrier(CLK_LOCAL_MEM_FENCE);\n */
/* if (get_local_id(0) < _stride) {\n */
/* _cl_local_mem_s1[get_local_id(0)] += _cl_local_mem_s1[get_local_id(0)+_stride];\n */
/* _cl_local_mem_s2[get_local_id(0)] += _cl_local_mem_s2[get_local_id(0)+_stride];\n */
/* }\n */
/* }\n */
/* if (get_global_id(0) == 0) {\n */
/* *_global_reduction_s1 = 0;\n */
/* *_global_reduction_s2 = 0;\n */
/* }\n */
/* barrier(CLK_GLOBAL_MEM_FENCE);\n */
/* if (get_local_id(0) == 0) {\n */
/* _GfnAtomicAddInt(_global_reduction_s1,_cl_local_mem_s1[0]); */
/* _GfnAtomicAddFloat(_global_reduction_s2,_cl_local_mem_s2[0]); */
/* }\n */
/* }\n */
/*  */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""void _GfnBarrier() {\n""    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n""}\n""int _GfnAtomicAddInt(__global int* const address, const int value) {\n""    return atomic_add(address, value);\n""}\n""float _GfnAtomicAddFloat(__global float* const address, const float value) {\n""    uint oldval, newval, readback;\n""    *(float*)&oldval = *address;\n""    *(float*)&newval = (*(float*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(float*)&newval = (*(float*)&oldval + value);\n""    }\n""    return *(float*)&oldval;\n""}\n""double _GfnAtomicAddDouble(__global double* const address, const double value) {\n""    long oldval, newval, readback;\n""    *(double*)&oldval = *address;\n""    *(double*)&newval = (*(double*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(double*)&newval = (*(double*)&oldval + value);\n""    }\n""    return *(double*)&oldval;\n""}\n""__kernel void _kernel_1(__global int * _global_reduction_s1,__local int * _cl_local_mem_s1,__global float * _global_reduction_s2,__local float * _cl_local_mem_s2,int _local_i_start,int _local_i_end,int _loop_step) {\n""int n;""int s1 = 0;\n""float s2 = 0;\n""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= n) {\n""{""    s1 += i;""    s2 += (float) i;""}\n""}\n""if (get_global_id(0) < _loop_size) {\n""_cl_local_mem_s1[get_local_id(0)] = s1;\n""_cl_local_mem_s2[get_local_id(0)] = s2;\n""} else {\n""_cl_local_mem_s1[get_local_id(0)] = 0;\n""_cl_local_mem_s2[get_local_id(0)] = 0;\n""}\n""for (int _stride = get_local_size(0)/2; _stride > 0; _stride /= 2) {\n""barrier(CLK_LOCAL_MEM_FENCE);\n""if (get_local_id(0) < _stride) {\n""_cl_local_mem_s1[get_local_id(0)] += _cl_local_mem_s1[get_local_id(0)+_stride];\n""_cl_local_mem_s2[get_local_id(0)] += _cl_local_mem_s2[get_local_id(0)+_stride];\n""}\n""}\n""if (get_global_id(0) == 0) {\n""*_global_reduction_s1 = 0;\n""*_global_reduction_s2 = 0;\n""}\n""barrier(CLK_GLOBAL_MEM_FENCE);\n""if (get_local_id(0) == 0) {\n""_GfnAtomicAddInt(_global_reduction_s1,_cl_local_mem_s1[0]);""_GfnAtomicAddFloat(_global_reduction_s2,_cl_local_mem_s2[0]);""}\n""}\n""";
void _Function_1()
{
    int i;
    int n;
    int _local_s1 = 0;
    float _local_s2 = 0;
    cl_mem _cl_mem_s1;
    cl_mem _cl_mem_s2;
    cl_kernel _kernel;
    /* Generated variable */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    int s1 = 0;
    float s2 = 0;
    if (_gfn_rank == 0)
    {
        _RecvInputMsg((void *) &n, sizeof(int));
    }
    MPI_Bcast(&n, 1, _get_mpi_int(), 0, _get_mpi_comm_world());
    /* Init generated variable */
    _local_i_start = _CalcLocalStartIndex(1, n, _gfn_num_proc, _gfn_rank + 1);
    _local_i_end = _CalcLocalEndIndex(1, n, _gfn_num_proc, _gfn_rank + 1);
    _loop_size = _CalcLoopSize(1, n, 1);
    _loop_step = 1;
    _cl_mem_s1 = clCreateBuffer(_gfn_context, _get_cl_mem_write_only(), sizeof(int), 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    _cl_mem_s2 = clCreateBuffer(_gfn_context, _get_cl_mem_write_only(), sizeof(float), 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    /* Send data to all process */
    /* Compute work-load */
    if (1)
    {
        size_t _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
        size_t _work_group_item_num = 64;
        size_t _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
        cl_int _local_cl_i_start = _local_i_start;
        cl_int _local_cl_i_end = _local_i_end;
        cl_int _cl_loop_step = _loop_step;

		printf("Rank %d have work item = %d\n", (int)_gfn_rank, (int)_work_item_num);
		printf("Rank %d locat start %d\n", (int)_gfn_rank, (int)_local_cl_i_start);
		printf("Rank %d local end = %d\n", (int)_gfn_rank, (int)_local_cl_i_end);
		
        /* TODO: Overlap node data transfer and device data transfer */
        _kernel = _CreateKernelFromSource("_kernel_1", _kernel_1_src, _gfn_context, _gfn_device_id);
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_s1);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(int) * _work_group_item_num, 0);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG (LOCAL SIZE)");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_s2);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(float) * _work_group_item_num, 0);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG (LOCAL SIZE)");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(cl_int), (void *) &_local_cl_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 5, sizeof(cl_int), (void *) &_local_cl_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 6, sizeof(cl_int), (void *) &_cl_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
        _gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, _cl_mem_s1, _get_cl_true(), 0, sizeof(int), &s1, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER &s1");
        _gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, _cl_mem_s2, _get_cl_true(), 0, sizeof(float), &s2, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER &s2");
    }
    else
    {
		printf("Rank %d , local start : %d\n", _gfn_rank, _local_i_start);
        for (i = _local_i_start;
            i < _local_i_end;
            i++ , _local_i += _loop_step)
        {
            s1 += i;
            s2 += (float) i;
        }
        printf("S1 local sum = %d\n", s1);
    }
    /* Gather data from all process */
    _local_s1 = s1;
    MPI_Reduce(&_local_s1, &s1, 1, _get_mpi_int(), _get_mpi_sum(), 0, _get_mpi_comm_world());
    _local_s2 = s2;
    MPI_Reduce(&_local_s2, &s2, 1, _get_mpi_float(), _get_mpi_sum(), 0, _get_mpi_comm_world());
    if (_gfn_rank == 0)
    {
        _SendOutputMsg((void *) &s1, sizeof(int));
        _SendOutputMsg((void *) &s2, sizeof(float));
    }
    _gfn_status = clReleaseMemObject(_cl_mem_s1);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_s1");
    _gfn_status = clReleaseMemObject(_cl_mem_s2);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_s2");
}
/* */ #endif /* */

#ifndef GFN_WORKER
int main(int argc, char *argv[])
{
    int i, *A, n = 5000;
    int expected_result = (n * (n + 1)) / 2;
    int error = 0;
    int s1 = 0;
    float s2 = 0;
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    /* Send call function message */
    _SendCallFuncMsg(1);
    _SendInputMsg((void *) &n, sizeof(int));
    _RecvOutputMsg((void *) &s1, sizeof(int));
    _RecvOutputMsg((void *) &s2, sizeof(float));
    if (expected_result != s1)
    {
        printf("s1 result error!! (Expected is [%d] but s1 is [%d])\n", expected_result, s1);
        error = 1;
    }
    if (expected_result != s2)
    {
        printf("s2 result error!! (Expected is [%d] but s2 is [%d])\n", expected_result, (int) s2);
        error = 1;
    }
    if (!error)
    {
        printf("TEST PASS!\n");
    }
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    return 0;
}
#endif

