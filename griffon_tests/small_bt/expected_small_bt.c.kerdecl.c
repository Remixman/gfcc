/* DEVICE_FUNCTION_DEFINITION func */
const char *_decl_func_src = "int func(int x);\n";
const char *_def_func_src = "int func(int x)""{""    return x * x;""}\n";

/* KERNEL_DEFINITION _kernel_1 */
const char *_kernel_1_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""__kernel void _kernel_1(int gp0,int gp1,int gp2,int ksize,int jsize,int isize,__global double * us,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            us[((i) * isize * jsize) + ((j) * isize) + (k)] = i + j + k;""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 1 _Function_1 */
void _Function_1()
{
    /* Declare Variables */
    int i;
    int gp0;
    int j;
    int gp1;
    int k;
    int gp2;
    int ksize;
    int jsize;
    int isize;
    double ***us;
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
    _GfnEnqueueBoardcastScalar(&ksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&jsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&isize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_us, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc3D((void ****) &us, &_cl_mem_us, _id_us, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (ksize) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (ksize) - 1);
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
        _kernel = _GfnCreateKernel("_kernel_1");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &gp0);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &gp1);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &gp2);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &ksize);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &jsize);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &isize);
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
    _GfnEnqueueGatherND((void *) us[0][0], _cl_mem_us, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_us, 1, 1);
}

/* KERNEL_DEFINITION _kernel_2 */
const char *_kernel_2_src = "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n""#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable\n""__kernel void _kernel_2(int gp0,int gp1,int gp2,int uksize,int ujsize,int uisize,int ksize,int jsize,int isize,__global double * qs,__global const double * u,__global double * rho_i,__global double * us,__global double * vs,__global double * ws,__global double * square,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""double rho_inv;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            rho_inv = 1.0 / u[((j) * ujsize * uksize) + ((k) * ujsize) + (0)];""            rho_i[((i) * isize * jsize) + ((j) * isize) + (k)] = rho_inv;""            us[((i) * isize * jsize) + ((j) * isize) + (k)] = u[((j) * ujsize * uksize) + ((k) * ujsize) + (1)] * rho_inv;""            vs[((i) * isize * jsize) + ((j) * isize) + (k)] = u[((j) * ujsize * uksize) + ((k) * ujsize) + (2)] * rho_inv;""            ws[((i) * isize * jsize) + ((j) * isize) + (k)] = u[((j) * ujsize * uksize) + ((k) * ujsize) + (3)] * rho_inv;""            square[((i) * isize * jsize) + ((j) * isize) + (k)] = 0.5 * (u[((j) * ujsize * uksize) + ((k) * ujsize) + (1)] * u[((j) * ujsize * uksize) + ((k) * ujsize) + (1)] + u[((j) * ujsize * uksize) + ((k) * ujsize) + (2)] * u[((j) * ujsize * uksize) + ((k) * ujsize) + (2)] + u[((j) * ujsize * uksize) + ((k) * ujsize) + (3)] * u[((j) * ujsize * uksize) + ((k) * ujsize) + (3)]) * rho_inv;""            qs[((i) * isize * jsize) + ((j) * isize) + (k)] = square[((i) * isize * jsize) + ((j) * isize) + (k)] * rho_inv;""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 2 _Function_2 */
void _Function_2()
{
    /* Declare Variables */
    int i;
    int gp0;
    int j;
    int gp1;
    int k;
    int gp2;
    double rho_inv;
    int uksize;
    int ujsize;
    int uisize;
    int ksize;
    int jsize;
    int isize;
    double ***qs;
    double ****u;
    double ***rho_i;
    double ***us;
    double ***vs;
    double ***ws;
    double ***square;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_i_start, _local_i_end;
    int _loop_step;
    int _loop_size;
    int _local_i = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_qs;
    cl_mem _cl_mem_qs = 0;
    long long _id_u;
    cl_mem _cl_mem_u = 0;
    long long _id_rho_i;
    cl_mem _cl_mem_rho_i = 0;
    long long _id_us;
    cl_mem _cl_mem_us = 0;
    long long _id_vs;
    cl_mem _cl_mem_vs = 0;
    long long _id_ws;
    cl_mem _cl_mem_ws = 0;
    long long _id_square;
    cl_mem _cl_mem_square = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&gp0, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp1, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp2, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&uksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ujsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&uisize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&jsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&isize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_qs, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_u, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_rho_i, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_us, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_vs, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_ws, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_square, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc3D((void ****) &qs, &_cl_mem_qs, _id_qs, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc4D((void *****) &u, &_cl_mem_u, _id_u, _GFN_TYPE_DOUBLE(), 5, uksize, ujsize, uisize, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &rho_i, &_cl_mem_rho_i, _id_rho_i, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &us, &_cl_mem_us, _id_us, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &vs, &_cl_mem_vs, _id_vs, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &ws, &_cl_mem_ws, _id_ws, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &square, &_cl_mem_square, _id_square, _GFN_TYPE_DOUBLE(), ksize, jsize, isize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (5) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (5) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (gp0) - 1);
    _loop_size = _CalcLoopSize(0, (gp0) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) u[0][0][0], _cl_mem_u, _GFN_TYPE_DOUBLE(), _GFN_MEM_READ_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 4, 0, 5, uksize, ujsize, uisize);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_2");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &gp0);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &gp1);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &gp2);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &uksize);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &ujsize);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &uisize);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &ksize);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &jsize);
        _GfnSetKernelArg(_kernel, 8, sizeof(int), (void *) &isize);
        _GfnSetKernelArg(_kernel, 9, sizeof(cl_mem), (void *) &_cl_mem_qs);
        _GfnSetKernelArg(_kernel, 10, sizeof(cl_mem), (void *) &_cl_mem_u);
        _GfnSetKernelArg(_kernel, 11, sizeof(cl_mem), (void *) &_cl_mem_rho_i);
        _GfnSetKernelArg(_kernel, 12, sizeof(cl_mem), (void *) &_cl_mem_us);
        _GfnSetKernelArg(_kernel, 13, sizeof(cl_mem), (void *) &_cl_mem_vs);
        _GfnSetKernelArg(_kernel, 14, sizeof(cl_mem), (void *) &_cl_mem_ws);
        _GfnSetKernelArg(_kernel, 15, sizeof(cl_mem), (void *) &_cl_mem_square);
        _GfnSetKernelArg(_kernel, 16, sizeof(int), (void *) &_local_i_start);
        _GfnSetKernelArg(_kernel, 17, sizeof(int), (void *) &_local_i_end);
        _GfnSetKernelArg(_kernel, 18, sizeof(int), (void *) &_loop_step);
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
                    rho_inv = 1.0 / u[i][j][k][0];
                    rho_i[i][j][k] = rho_inv;
                    us[i][j][k] = u[i][j][k][1] * rho_inv;
                    vs[i][j][k] = u[i][j][k][2] * rho_inv;
                    ws[i][j][k] = u[i][j][k][3] * rho_inv;
                    square[i][j][k] = 0.5 * (u[i][j][k][1] * u[i][j][k][1] + u[i][j][k][2] * u[i][j][k][2] + u[i][j][k][3] * u[i][j][k][3]) * rho_inv;
                    qs[i][j][k] = square[i][j][k] * rho_inv;
                }
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) qs[0][0], _cl_mem_qs, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnEnqueueGatherND((void *) rho_i[0][0], _cl_mem_rho_i, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnEnqueueGatherND((void *) us[0][0], _cl_mem_us, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnEnqueueGatherND((void *) vs[0][0], _cl_mem_vs, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnEnqueueGatherND((void *) ws[0][0], _cl_mem_ws, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnEnqueueGatherND((void *) square[0][0], _cl_mem_square, _GFN_TYPE_DOUBLE(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, ksize, jsize, isize);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_qs, 1, 1);
    _GfnFree(_id_u, 1, 1);
    _GfnFree(_id_rho_i, 1, 1);
    _GfnFree(_id_us, 1, 1);
    _GfnFree(_id_vs, 1, 1);
    _GfnFree(_id_ws, 1, 1);
    _GfnFree(_id_square, 1, 1);
}

