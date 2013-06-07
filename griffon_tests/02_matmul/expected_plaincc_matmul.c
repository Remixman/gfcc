long long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, ((void *) 0));
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
void init(int n, float **A, float **B)
{
    int i, j;
    srand(time(((void *) 0)));
    for (i = 0;
        i < n;
        i++)
    {
        for (j = 0;
            j < n;
            j++)
        {
            A[i][j] = (rand() % 100) / 100.f;
            B[i][j] = (rand() % 100) / 100.f;
        }
    }
}
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
/* __kernel void _kernel_1(__global float * C,__global const float * A,__global const float * B,int _local_i_start,int _local_i_end,int _loop_step) {\n */
/* int n; */
/* int j; */
/* int k; */
/* int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n */
/* int _local_i = get_global_id(0) * _loop_step;\n */
/* int i = _local_i + _local_i_start;\n */
/* if (i <= (n) - 1) {\n */
/* { */
/*     for (j = 0; */
/*         j < n; */
/*         j++) */
/*     { */
/*         C[((_local_i) * n) + (j)] = 0.f; */
/*         for (k = 0; */
/*             k < n; */
/*             k++) */
/*         { */
/*             C[((_local_i) * n) + (j)] += A[((_local_i) * n) + (k)] * B[((k) * n) + (j)]; */
/*         } */
/*     } */
/* }\n */
/* }\n */
/* }\n */
/*  */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""void _GfnBarrier() {\n""    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n""}\n""int _GfnAtomicAddInt(__global int* const address, const int value) {\n""    return atomic_add(address, value);\n""}\n""float _GfnAtomicAddFloat(__global float* const address, const float value) {\n""    uint oldval, newval, readback;\n""    *(float*)&oldval = *address;\n""    *(float*)&newval = (*(float*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(float*)&newval = (*(float*)&oldval + value);\n""    }\n""    return *(float*)&oldval;\n""}\n""double _GfnAtomicAddDouble(__global double* const address, const double value) {\n""    long oldval, newval, readback;\n""    *(double*)&oldval = *address;\n""    *(double*)&newval = (*(double*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(double*)&newval = (*(double*)&oldval + value);\n""    }\n""    return *(double*)&oldval;\n""}\n""__kernel void _kernel_1(__global float * C,__global const float * A,__global const float * B,int _local_i_start,int _local_i_end,int _loop_step) {\n""int n;""int j;""int k;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (n) - 1) {\n""{""    for (j = 0;""        j < n;""        j++)""    {""        C[((_local_i) * n) + (j)] = 0.f;""        for (k = 0;""            k < n;""            k++)""        {""            C[((_local_i) * n) + (j)] += A[((_local_i) * n) + (k)] * B[((k) * n) + (j)];""        }""    }""}\n""}\n""}\n""";
void _Function_1()
{
    int i;
    int n;
    int j;
    int k;
    float *C;
    float *A;
    float *B;
    cl_mem _cl_mem_C;
    cl_mem _cl_mem_A;
    cl_mem _cl_mem_B;
    cl_kernel _kernel;
    /* Generated variable */
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    float *_buffer_C;
    int _sub_size_C;
    int _C_cnts[_gfn_num_proc];
    int _C_disp[_gfn_num_proc];
    float *_buffer_A;
    int _sub_size_A;
    int _A_cnts[_gfn_num_proc];
    int _A_disp[_gfn_num_proc];
    if (_gfn_rank == 0)
    {
        _RecvInputMsg((void *) &n, sizeof(int));
    }
    MPI_Bcast(&n, 1, _get_mpi_int(), 0, _get_mpi_comm_world());
    B = (float *) malloc((sizeof(float) * ((n) * (n))));
    if (_gfn_rank == 0)
    {
        _buffer_C = (float *) malloc((sizeof(float) * ((n) * (n))));
        _buffer_A = (float *) malloc((sizeof(float) * ((n) * (n))));
        _RecvInputMsg((void *) _buffer_A, (sizeof(float) * ((n) * (n))));
        _RecvInputMsg((void *) B, (sizeof(float) * ((n) * (n))));
    }
    /* Init generated variable */
    _local_i_start = _CalcLocalStartIndex(0, (n) - 1, _gfn_num_proc, _gfn_rank + 1);
    _local_i_end = _CalcLocalEndIndex(0, (n) - 1, _gfn_num_proc, _gfn_rank + 1);
    _loop_size = _CalcLoopSize(0, (n) - 1, 1);
    _loop_step = 1;
    _sub_size_C = _CalcSubSize((n), _gfn_num_proc, _gfn_rank, ((n) * (1)));
    C = (float *) malloc(sizeof(float) * _sub_size_C);
    _CalcCnts((n), _gfn_num_proc, _C_cnts, ((n) * (1)));
    _CalcDisp((n), _gfn_num_proc, _C_disp, ((n) * (1)));
    _sub_size_A = _CalcSubSize((n), _gfn_num_proc, _gfn_rank, ((n) * (1)));
    A = (float *) malloc(sizeof(float) * _sub_size_A);
    _CalcCnts((n), _gfn_num_proc, _A_cnts, ((n) * (1)));
    _CalcDisp((n), _gfn_num_proc, _A_disp, ((n) * (1)));
    _cl_mem_C = clCreateBuffer(_gfn_context, _get_cl_mem_write_only(), sizeof(float) * _sub_size_C, 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    _cl_mem_A = clCreateBuffer(_gfn_context, _get_cl_mem_read_only(), sizeof(float) * _sub_size_A, 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    _cl_mem_B = clCreateBuffer(_gfn_context, _get_cl_mem_read_only(), (sizeof(float) * ((n) * (n))), 0, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE BUFFER 0");
    /* Send data to all process */
    MPI_Scatterv(_buffer_A, _A_cnts, _A_disp, _get_mpi_float(), A, _sub_size_A, _get_mpi_float(), 0, _get_mpi_comm_world());
    MPI_Bcast(B, ((n) * (n)), _get_mpi_float(), 0, _get_mpi_comm_world());
    /* Compute work-load */
    if (0)
    {
        size_t _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
        size_t _work_group_item_num = 64;
        size_t _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
        cl_int _local_cl_i_start = _local_i_start;
        cl_int _local_cl_i_end = _local_i_end;
        cl_int _cl_loop_step = _loop_step;
        /* TODO: Overlap node data transfer and device data transfer */
        _gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, _cl_mem_A, _get_cl_true(), 0, sizeof(float) * _sub_size_A, A, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "WRITE BUFFER A");
        _gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, _cl_mem_B, _get_cl_true(), 0, (sizeof(float) * ((n) * (n))), B, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "WRITE BUFFER B");
        _kernel = _CreateKernelFromSource("_kernel_1", _kernel_1_src, _gfn_context, _gfn_device_id);
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_C);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(cl_int), (void *) &_local_cl_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(cl_int), (void *) &_local_cl_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 5, sizeof(cl_int), (void *) &_cl_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
        _gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, _cl_mem_C, _get_cl_true(), 0, sizeof(float) * _sub_size_C, C, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "READ BUFFER C");
    }
    else
    {
        for (i = _local_i_start;
            i < _local_i_end;
            i++ , _local_i += _loop_step)
        {
            for (j = 0;
                j < n;
                j++)
            {
                C[((_local_i) * n) + (j)] = 0.f;
                for (k = 0;
                    k < n;
                    k++)
                {
                    C[((_local_i) * n) + (j)] += A[((_local_i) * n) + (k)] * B[((k) * n) + (j)];
                }
            }
        }
    }
    /* Gather data from all process */
    MPI_Gatherv(C, _sub_size_C, _get_mpi_float(), _buffer_C, _C_cnts, _C_disp, _get_mpi_float(), 0, _get_mpi_comm_world());
    if (_gfn_rank == 0)
    {
        _SendOutputMsg((void *) _buffer_C, (sizeof(float) * ((n) * (n))));
    }
    free(C);
    free(A);
    free(B);
    if (_gfn_rank == 0)
    {
        free(_buffer_C);
        free(_buffer_A);
    }
    _gfn_status = clReleaseMemObject(_cl_mem_C);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_C");
    _gfn_status = clReleaseMemObject(_cl_mem_A);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_A");
    _gfn_status = clReleaseMemObject(_cl_mem_B);
    _GfnCheckCLStatus(_gfn_status, "RELEASE BUFFER _cl_mem_B");
}
/* */ #endif /* */
void matmul_kernel(int n, float **A, float **B, float **C)
{
    int i, j, k;
    /* Send call function message */
    _SendCallFuncMsg(1);
    _SendInputMsg((void *) &n, sizeof(int));
    _SendInputMsg((void *) A[0], (sizeof(float) * ((n) * (n))));
    _SendInputMsg((void *) B[0], (sizeof(float) * ((n) * (n))));
    _RecvOutputMsg((void *) C[0], (sizeof(float) * ((n) * (n))));
}
#ifndef GFN_WORKER
int main(int argc, char *argv[])
{
    int ite, i, j, k;
    int n, pass = 1;
    float **A, **B, **C, sum;
    long long time0, time1;
    n = 1000;
    ite = 10;
    if (argc > 1)
        n = atoi(argv[1]);
    if (argc > 2)
        ite = atoi(argv[2]);
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    A = (float **) malloc(n * sizeof(float *));
    A[0] = (float *) malloc(n * n * sizeof(float));
    B = (float **) malloc(n * sizeof(float *));
    B[0] = (float *) malloc(n * n * sizeof(float));
    C = (float **) malloc(n * sizeof(float *));
    C[0] = (float *) malloc(n * n * sizeof(float));
    for (i = 1;
        i < n;
        i++)
    {
        A[i] = A[i - 1] + n;
        B[i] = B[i - 1] + n;
        C[i] = C[i - 1] + n;
    }
    init(n, A, B);
    matmul_kernel(n, A, B, C);
    time0 = get_time();
    for (i = 0;
        i < ite;
        i++)
        matmul_kernel(n, A, B, C);
    time1 = get_time();
    for (i = 0;
        i < n;
        i++)
    {
        for (j = 0;
            j < n;
            j++)
        {
            sum = 0.f;
            for (k = 0;
                k < n;
                k++)
            {
                sum += A[i][k] * B[k][j];
            }
            if (sum != C[i][j])
            {
                pass = 0;
                break;
            }
        }
        if (!pass)
            break;
    }
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    printf("TEST 02 - Matrix Matrix Multiplication\n");
    printf("\tTest result = ");
    if (pass)
        printf("SUCCESSFUL\n");
    else
        printf("FAILURE\n");
    printf("\tProblem size = %d x %d\n", n, n);
    printf("\tRunning iteration = %d\n", ite);
    printf("\tAverage time = %f sec.\n", ((float) (time1 - time0) / 1000000) / ite);
    return 0;
}
#endif

