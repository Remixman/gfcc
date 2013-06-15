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
/* __kernel void _kernel_1(int n,__global float * C,__global const float * A,__global const float * B,int _local_i_start,int _local_i_end,int _loop_step) {\n */
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
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""void _GfnBarrier() {\n""    barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);\n""}\n""int _GfnAtomicAddInt(__global int* const address, const int value) {\n""    return atomic_add(address, value);\n""}\n""float _GfnAtomicAddFloat(__global float* const address, const float value) {\n""    uint oldval, newval, readback;\n""    *(float*)&oldval = *address;\n""    *(float*)&newval = (*(float*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global uint*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(float*)&newval = (*(float*)&oldval + value);\n""    }\n""    return *(float*)&oldval;\n""}\n""double _GfnAtomicAddDouble(__global double* const address, const double value) {\n""    long oldval, newval, readback;\n""    *(double*)&oldval = *address;\n""    *(double*)&newval = (*(double*)&oldval + value);\n""    while ((readback = atom_cmpxchg((__global long*)address, oldval, newval)) != oldval) {\n""        oldval = readback;\n""        *(double*)&newval = (*(double*)&oldval + value);\n""    }\n""    return *(double*)&oldval;\n""}\n""__kernel void _kernel_1(int n,__global float * C,__global const float * A,__global const float * B,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (n) - 1) {\n""{""    for (j = 0;""        j < n;""        j++)""    {""        C[((_local_i) * n) + (j)] = 0.f;""        for (k = 0;""            k < n;""            k++)""        {""            C[((_local_i) * n) + (j)] += A[((_local_i) * n) + (k)] * B[((k) * n) + (j)];""        }""    }""}\n""}\n""}\n""";
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
    _GfnEnqueueBoardcastScalar(&n, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_C, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_A, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_B, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc2D((void ***) &C, _cl_mem_C, _id_C, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_WRITE_ONLY(), 1, 0);
    _GfnMalloc2D((void ***) &A, _cl_mem_A, _id_A, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_ONLY(), 1, 0);
    _GfnMalloc2D((void ***) &B, _cl_mem_B, _id_B, _GFN_TYPE_FLOAT(), n, n, _GFN_MEM_READ_ONLY(), 1, 0);
    /* Initialize Generated Variables */
    _local_i_start = _CalcLocalStartIndex(0, (n) - 1, _gfn_num_proc, _gfn_rank + 1);
    _local_i_end = _CalcLocalEndIndex(0, (n) - 1, _gfn_num_proc, _gfn_rank + 1);
    _loop_size = _CalcLoopSize(0, (n) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Distribute Array Memory */
    _GfnEnqueueScatter2D((void ***) &A, _cl_mem_A, _GFN_TYPE_FLOAT(), 1, n, n, 0, 0, 1, 0);
    _GfnEnqueueBoardcast2D((void ***) &B, _cl_mem_B, _GFN_TYPE_FLOAT(), n, n, 1, 0);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (0)
    {
        _kernel = _CreateKernelFromSource("_kernel_1", _kernel_1_src, _gfn_context, _gfn_device_id);
        _gfn_status = clSetKernelArg(_kernel, 0, sizeof(cl_mem), (void *) &_cl_mem_C);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 1, sizeof(cl_mem), (void *) &_cl_mem_A);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 2, sizeof(cl_mem), (void *) &_cl_mem_B);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 3, sizeof(cl_int), (void *) &_local_i_start);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 4, sizeof(cl_int), (void *) &_local_i_end);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clSetKernelArg(_kernel, 5, sizeof(cl_int), (void *) &_loop_step);
        _GfnCheckCLStatus(_gfn_status, "SET KERNEL ARG");
        _gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue, _kernel, 1, 0, &_global_item_num, &_work_group_item_num, 0, 0, 0);
        _GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");
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
    _GfnEnqueueGather2D((void ***) &C, _cl_mem_C, _GFN_TYPE_FLOAT(), 1, n, n, 0, 0, 1, 0);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
}
/* */ #endif /* */
void matmul_kernel(int n, float **A, float **B, float **C)
{
    int i, j, k;
    /* Send call function message */
    _SendCallFuncMsg(1);
    _SendInputMsg((void *) &n, sizeof(int));
    _SendInputMsg((void *) n, sizeof(int));
    _SendConstInputMsg((long long) &C);
    _SendConstInputMsg((long long) &A);
    _SendConstInputMsg((long long) &B);
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
            if (fabs(sum - C[i][j]) > 0.0001)
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

