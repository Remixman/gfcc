/* */ #ifdef GFN_WORKER /* */
/* #pragma OPENCL EXTENSION cl_khr_fp64 : enable\n */
/* #pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n */
/* void _GfnBarrier() { */
/*     barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE); */
/* } */
/*  */
/* int _GfnAtomicAddInt(__global int* const address, const int value) { */
/*     return atomic_add(address, value); */
/* } */
/*  */
/* float _GfnAtomicAddFloat(__global float* const address, const float value) { */
/*     uint oldval, newval, readback; */
/*     *(float*)&oldval = *address; */
/*     *(float*)&newval = (*(float*)&oldval + value); */
/*     while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) { */
/*         oldval = readback; */
/*         *(float*)&newval = (*(float*)&oldval + value); */
/*     } */
/*     return *(float*)&oldval; */
/* } */
/*  */
/* double _GfnAtomicAddDouble(__global double* const address, const double value) { */
/*     long oldval, newval, readback; */
/*     *(double*)&oldval = *address; */
/*     *(double*)&newval = (*(double*)&oldval + value); */
/*     while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) { */
/*         oldval = readback; */
/*         *(double*)&newval = (*(double*)&oldval + value); */
/*     } */
/*     return *(double*)&oldval; */
/* } */
/*  */
/* __kernel void _kernel_1(__global int * A,__global const int * B,int _local_i_start,int _local_i_end,int _loop_step) { */
/* int _loop_size = (_local_i_end - _local_i_start) / _loop_step; */
/* int _local_i = get_global_id(0) * _loop_step; */
/* int i = _local_i + _local_i_start; */
/* if (i <= (300) - 1) { */
/* { */
/*     A[(_local_i)] = B[(_local_i)]; */
/*     _GfnBarrier(); */
/*     A[(_local_i)] *= - 1; */
/* } */
/* } */
/* } */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""void _GfnBarrier() {""    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);""}""""int _GfnAtomicAddInt(__global int* const address, const int value) {""    return atomic_add(address, value);""}""""float _GfnAtomicAddFloat(__global float* const address, const float value) {""    uint oldval, newval, readback;""    *(float*)&oldval = *address;""    *(float*)&newval = (*(float*)&oldval + value);""    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {""        oldval = readback;""        *(float*)&newval = (*(float*)&oldval + value);""    }""    return *(float*)&oldval;""}""""double _GfnAtomicAddDouble(__global double* const address, const double value) {""    long oldval, newval, readback;""    *(double*)&oldval = *address;""    *(double*)&newval = (*(double*)&oldval + value);""    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {""        oldval = readback;""        *(double*)&newval = (*(double*)&oldval + value);""    }""    return *(double*)&oldval;""}""""__kernel void _kernel_1(__global int * A,__global const int * B,int _local_i_start,int _local_i_end,int _loop_step) {""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;""int _local_i = get_global_id(0) * _loop_step;""int i = _local_i + _local_i_start;""if (i <= (300) - 1) {""{""    A[(_local_i)] = B[(_local_i)];""    _GfnBarrier();""    A[(_local_i)] *= - 1;""}""}""}";
void _Function_1()
{
    int i;
    int *A;
    int *B;
    cl_mem _cl_mem_A;
    cl_mem _cl_mem_B;
    cl_kernel _kernel;
    /* Generated variable */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    int *_buffer_A;
    int _sub_size_A;
    int _A_cnts[_gfn_num_proc];
    int _A_disp[_gfn_num_proc];
    int *_buffer_B;
    int _sub_size_B;
    int _B_cnts[_gfn_num_proc];
    int _B_disp[_gfn_num_proc];
    if (_gfn_rank == 0)
    {
        _buffer_A = (int *) malloc((sizeof(int) * ((300))));
        _buffer_B = (int *) malloc((sizeof(int) * ((300))));
        _RecvInputMsg((void *) _buffer_B, (sizeof(int) * ((300))));
    }
    /* Init generated variable */
    _local_i_start = _CalcLocalStartIndex(0, (300) - 1, _gfn_num_proc, _gfn_rank + 1);
    _local_i_end = _CalcLocalEndIndex(0, (300) - 1, _gfn_num_proc, _gfn_rank + 1);
    _loop_size = _CalcLoopSize(0, (300) - 1, 1);
    _loop_step = 1;
    _sub_size_A = _CalcSubSize((300), _gfn_num_proc, _gfn_rank, ((1) * (1)));
    A = (int *) malloc(sizeof(int) * _sub_size_A);
    _CalcCnts((300), _gfn_num_proc, _A_cnts, ((1) * (1)));
    _CalcDisp((300), _gfn_num_proc, _A_disp, ((1) * (1)));
    _sub_size_B = _CalcSubSize((300), _gfn_num_proc, _gfn_rank, ((1) * (1)));
    B = (int *) malloc(sizeof(int) * _sub_size_B);
    _CalcCnts((300), _gfn_num_proc, _B_cnts, ((1) * (1)));
    _CalcDisp((300), _gfn_num_proc, _B_disp, ((1) * (1)));
    _cl_mem_A = clCreateBuffer(_gfn_context, _get_cl_mem_write_only(), sizeof(int) * _sub_size_A, 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    _cl_mem_B = clCreateBuffer(_gfn_context, _get_cl_mem_read_only(), sizeof(int) * _sub_size_B, 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    /* Send data to all process */
    MPI_Scatterv(_buffer_B, _B_cnts, _B_disp, _get_mpi_int(), B, _sub_size_B, _get_mpi_int(), 0, _get_mpi_comm_world());
    /* Compute work-load */
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

        /* TODO: Overlap node data transfer and device data transfer */
        _gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, _cl_mem_B, _get_cl_true(), 0, sizeof(int) * _sub_size_B, B, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "WRITE BUFFER B");
        _kernel = _CreateKernelFromSource("_kernel_1", _kernel_1_src, _gfn_context, _gfn_device_id);
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_int), (void *) &_local_cl_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(cl_int), (void *) &_local_cl_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(cl_int), (void *) &_cl_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
        _gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, _cl_mem_A, _get_cl_true(), 0, sizeof(int) * _sub_size_A, A, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER A");

        printf("LOCAL BUFFER A OF RANK %d : ", _gfn_rank);
        for(i = 0; i < _work_item_num; i++) printf("%d ", _local_buffer_A[i]);
        printf("\n\n");
    }
    else
    {
        for (i = _local_i_start;
            i < _local_i_end;
            i++ , _local_i += _loop_step)
        {
            A[(_local_i)] = B[(_local_i)];
            _GfnBarrier();
            A[(_local_i)] *= - 1;
        }
    }
    
    printf("LOCAL BUFFER : \n");
    for (i = 0; i < 75; i++) printf("%d, ", _local_buffer_A[i]);
    printf("\n\n");
    
    /* Gather data from all process */
    MPI_Gatherv(A, _sub_size_A, _get_mpi_int(), _buffer_A, _A_cnts, _A_disp, _get_mpi_int(), 0, _get_mpi_comm_world());
    if (_gfn_rank == 0)
    {
    	printf("GLOBAL BUFFER : \n");
    	for (i = 0; i < 300; i++) {
    		printf("%d, ", _buffer_A[i]);
    	} printf("\n\n");
        _SendOutputMsg((void *) _buffer_A, (sizeof(int) * ((300))));
    }
    free(A);
    free(B);
    if (_gfn_rank == 0)
    {
        free(_buffer_A);
        free(_buffer_B);
    }
    _gfn_status = clReleaseMemObject(_cl_mem_A);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_A");
    _gfn_status = clReleaseMemObject(_cl_mem_B);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_B");
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
    _SendInputMsg((void *) B, (sizeof(int) * ((300))));
    _RecvOutputMsg((void *) A, (sizeof(int) * ((300))));
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

