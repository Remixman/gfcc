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
/* __kernel void _kernel_1(__global int * A,__global const int * B,int _local_i_start,int _local_i_end,int _loop_step) {\n */
/* int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n */
/* int _local_i = get_global_id(0) * _loop_step;\n */
/* int i = _local_i + _local_i_start;\n */
/* if (i <= (300) - 1) {\n */
/* { */
/*     A[(i)] = B[(i)]; */
/*     A[(i)] *= - 1; */
/* }\n */
/* }\n */
/* }\n */
/*  */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""void _GfnBarrier() {\n""    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n""}\n""int _GfnAtomicAddInt(__global int* const address, const int value) {\n""    return atomic_add(address, value);\n""}\n""float _GfnAtomicAddFloat(__global float* const address, const float value) {\n""    uint oldval, newval, readback;\n""    *(float*)&oldval = *address;\n""    *(float*)&newval = (*(float*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(float*)&newval = (*(float*)&oldval + value);\n""    }\n""    return *(float*)&oldval;\n""}\n""double _GfnAtomicAddDouble(__global double* const address, const double value) {\n""    long oldval, newval, readback;\n""    *(double*)&oldval = *address;\n""    *(double*)&newval = (*(double*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(double*)&newval = (*(double*)&oldval + value);\n""    }\n""    return *(double*)&oldval;\n""}\n""__kernel void _kernel_1(__global int * A,__global const int * B,int _local_i_start,int _local_i_end,int _loop_step) {\n""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (300) - 1) {\n""{""    A[(i)] = B[(i)];""    A[(i)] *= - 1;""}\n""}\n""}\n""";
void _Function_1()
{
    /* Declare Variables */
    int i;
    int *A;
    int *B;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_A;
    cl_mem _cl_mem_A = 0;
    long long _id_B;
    cl_mem _cl_mem_B = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc1D((void **) &A, &_cl_mem_A, _id_A, _GFN_TYPE_INT(), 300, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc1D((void **) &B, &_cl_mem_B, _id_B, _GFN_TYPE_INT(), 300, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (300) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (300) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (300) - 1);
    _loop_size = _CalcLoopSize(0, (300) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Distribute Array Memory */
    _GfnEnqueueScatter1D((void **) &B, _cl_mem_B, _GFN_TYPE_INT(), 0, (300) - 1, 1, 1, 300, _GFN_MEM_READ_ONLY(), 0, 0, 0, 1, 1);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        size_t _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
        size_t _work_group_item_num = 64;
        size_t _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
        cl_int _local_cl_i_start = _local_i_start;
        cl_int _local_cl_i_end = _local_i_end;
        cl_int _cl_loop_step = _loop_step;
        printf("LOCAL BUFFER B OF RANK %d : ", _gfn_rank);
        for(i = 0; i < _work_item_num; i++) printf("%d ", _local_buffer_B[i]);
        printf("\n\n");

        _kernel = _GfnCreateKernel("_kernel_1", _kernel_1_src, _gfn_context, _gfn_device_id);
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(int), (void *) &_local_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(int), (void *) &_local_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(int), (void *) &_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
        _GfnClearKernel(_kernel);

        printf("LOCAL BUFFER A OF RANK %d : ", _gfn_rank);
        for(i = 0; i < _work_item_num; i++) printf("%d ", _local_buffer_A[i]);
        printf("\n\n");
    }
    else
    {
        for (i = _local_i_start;
            i <= _local_i_end;
            i++ , _local_i += _loop_step)
        {
            A[i] = B[i];
            A[i] *= - 1;
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGather1D((void **) &A, _cl_mem_A, _GFN_TYPE_INT(), 0, (300) - 1, 1, 1, 300, _GFN_MEM_WRITE_ONLY(), 0, 0, 0, 1, 1);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_A, 1, 1);
    _GfnFree(_id_B, 1, 1);
}
/* */ #endif /* */

#ifndef GFN_WORKER
int main(int argc, char *argv[])
{
	//int id = fork();
	
	//if (id == 0) {
	//system("mpirun -np 4 worker");
	//} else {
    int i, A[300], B[300] = {}, u = 99, it = - 2;
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    for (i = 0;
        i < 300;
        i++)
    {
        B[i] = i;
    }
    /* Send call function message */
    _SendCallFuncMsg(1);
    _SendConstInputMsg((long long) &A);
    _SendConstInputMsg((long long) &B);
    _SendInputNDMsg(&(B[0]), _GFN_TYPE_INT(), 0, (300) - 1, 1, 1, 0, 1, 0, 300);
    _RecvOutputNDMsg(&(A[0]), _GFN_TYPE_INT(), 0, (300) - 1, 1, 1, 0, 1, 0, 1);
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    for (i = 0;
        i < 300;
        i++)
    {
        if (A[i] != (- 1 * B[i]))
        {
            printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, - 1 * B[i], A[i]);
            ((0) ? (void) (0) : __assert_fail("0", "simple.c", 27, __PRETTY_FUNCTION__));
        }
    }
    printf("TEST PASS!\n");
    return 0;
    //}
}
#endif

