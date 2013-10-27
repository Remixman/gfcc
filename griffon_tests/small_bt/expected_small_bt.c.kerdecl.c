/* KERNEL_DEFINITION _kernel_837377 */
const char *_kernel_837377_src = "__kernel void _kernel_837377(int gp0,int gp1,int gp2,int isize,int jsize,int ksize,__global float * us,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            us[((i) * ksize * jsize) + ((j) * ksize) + (k)] = i + j + k;""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 837377 _Function_837377 */
void _Function_837377()
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
        _kernel = _GfnCreateKernel("_kernel_837377");
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

/* KERNEL_DEFINITION _kernel_164222 */
const char *_kernel_164222_src = "__kernel void _kernel_164222(int gp0,int gp1,int gp2,int isize,int jsize,int ksize,__global float * lhs,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""int m;""int n;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            for (m = 0;""                m < 5;""                m++)""            {""                for (n = 0;""                    n < 5;""                    n++)""                {""                    lhs[((0) * ksize * jsize) + ((m) * ksize) + (n)] = 0.0;""                    lhs[((1) * ksize * jsize) + ((m) * ksize) + (n)] = 0.0;""                    lhs[((2) * ksize * jsize) + ((m) * ksize) + (n)] = 0.0;""                }""            }""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 164222 _Function_164222 */
void _Function_164222()
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
        _kernel = _GfnCreateKernel("_kernel_164222");
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
                    for (m = 0;
                        m < 5;
                        m++)
                    {
                        for (n = 0;
                            n < 5;
                            n++)
                        {
                            lhs[i][j][k][0][m][n] = 0.0;
                            lhs[i][j][k][1][m][n] = 0.0;
                            lhs[i][j][k][2][m][n] = 0.0;
                        }
                    }
                }
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) lhs[0][0][0][0][0], _cl_mem_lhs, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 6, 0, isize, jsize, ksize, 3, 5, 5);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_lhs, 1, 1);
}

/* KERNEL_DEFINITION _kernel_982620 */
const char *_kernel_982620_src = "__kernel void _kernel_982620(int gp0,int gp1,int gp2,int uisize,int ujsize,int uksize,int isize,int jsize,int ksize,__global float * qs,__global const float * u,__global float * rho_i,__global float * us,__global float * vs,__global float * ws,__global float * square,int _local_i_start,int _local_i_end,int _loop_step) {\n""int j;""int k;""float rho_inv;""int _loop_size = (_local_i_end - _local_i_start) / _loop_step;\n""int _local_i = get_global_id(0) * _loop_step;\n""int i = _local_i + _local_i_start;\n""if (i <= (gp0) - 1) {\n""{""    for (j = 0;""        j < gp1;""        j++)""    {""        for (k = 0;""            k < gp2;""            k++)""        {""            rho_inv = 1.0 / u[((j) * uksize * ujsize) + ((k) * uksize) + (0)];""            rho_i[((i) * ksize * jsize) + ((j) * ksize) + (k)] = rho_inv;""            us[((i) * ksize * jsize) + ((j) * ksize) + (k)] = u[((j) * uksize * ujsize) + ((k) * uksize) + (1)] * rho_inv;""            vs[((i) * ksize * jsize) + ((j) * ksize) + (k)] = u[((j) * uksize * ujsize) + ((k) * uksize) + (2)] * rho_inv;""            ws[((i) * ksize * jsize) + ((j) * ksize) + (k)] = u[((j) * uksize * ujsize) + ((k) * uksize) + (3)] * rho_inv;""            square[((i) * ksize * jsize) + ((j) * ksize) + (k)] = 0.5 * (u[((j) * uksize * ujsize) + ((k) * uksize) + (1)] * u[((j) * uksize * ujsize) + ((k) * uksize) + (1)] + u[((j) * uksize * ujsize) + ((k) * uksize) + (2)] * u[((j) * uksize * ujsize) + ((k) * uksize) + (2)] + u[((j) * uksize * ujsize) + ((k) * uksize) + (3)] * u[((j) * uksize * ujsize) + ((k) * uksize) + (3)]) * rho_inv;""            qs[((i) * ksize * jsize) + ((j) * ksize) + (k)] = square[((i) * ksize * jsize) + ((j) * ksize) + (k)] * rho_inv;""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 982620 _Function_982620 */
void _Function_982620()
{
    /* Declare Variables */
    int i;
    int gp0;
    int j;
    int gp1;
    int k;
    int gp2;
    float rho_inv;
    int uisize;
    int ujsize;
    int uksize;
    int isize;
    int jsize;
    int ksize;
    float ***qs;
    float ****u;
    float ***rho_i;
    float ***us;
    float ***vs;
    float ***ws;
    float ***square;
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
    _GfnEnqueueBoardcastScalar(&uisize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ujsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&uksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&isize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&jsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_qs, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_u, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_rho_i, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_us, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_vs, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_ws, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_square, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc3D((void ****) &qs, &_cl_mem_qs, _id_qs, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc4D((void *****) &u, &_cl_mem_u, _id_u, _GFN_TYPE_FLOAT(), uisize, ujsize, uksize, 5, _GFN_MEM_READ_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &rho_i, &_cl_mem_rho_i, _id_rho_i, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &us, &_cl_mem_us, _id_us, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &vs, &_cl_mem_vs, _id_vs, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &ws, &_cl_mem_ws, _id_ws, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    _GfnMalloc3D((void ****) &square, &_cl_mem_square, _id_square, _GFN_TYPE_FLOAT(), isize, jsize, ksize, _GFN_MEM_WRITE_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (uisize) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (uisize) - 1);
    _local_i_start = _GfnCalcLocalLoopStart(_local_data_start, 0, 1);
    _local_i_end = _GfnCalcLocalLoopEnd(_local_data_end, (gp0) - 1);
    _loop_size = _CalcLoopSize(0, (gp0) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) u[0][0][0], _cl_mem_u, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 4, 0, uisize, ujsize, uksize, 5);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_982620");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &gp0);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &gp1);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &gp2);
        _GfnSetKernelArg(_kernel, 3, sizeof(int), (void *) &uisize);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &ujsize);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &uksize);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &isize);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &jsize);
        _GfnSetKernelArg(_kernel, 8, sizeof(int), (void *) &ksize);
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
    _GfnEnqueueGatherND((void *) qs[0][0], _cl_mem_qs, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
    _GfnEnqueueGatherND((void *) rho_i[0][0], _cl_mem_rho_i, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
    _GfnEnqueueGatherND((void *) us[0][0], _cl_mem_us, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
    _GfnEnqueueGatherND((void *) vs[0][0], _cl_mem_vs, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
    _GfnEnqueueGatherND((void *) ws[0][0], _cl_mem_ws, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
    _GfnEnqueueGatherND((void *) square[0][0], _cl_mem_square, _GFN_TYPE_FLOAT(), _GFN_MEM_WRITE_ONLY(), 0, (gp0) - 1, 1, 0, 0, 1, 1, 3, 0, isize, jsize, ksize);
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

/* KERNEL_DEFINITION _kernel_248767 */
const char *_kernel_248767_src = "__kernel void _kernel_248767(int gp1,int gp2,int i,float dssp,int uisize,int ujsize,int uksize,int isize,int jsize,int ksize,__global float * rhs,__global const float * u,int _local_j_start,int _local_j_end,int _loop_step) {\n""int k;""int m;""int _loop_size = (_local_j_end - _local_j_start) / _loop_step;\n""int _local_j = get_global_id(0) * _loop_step;\n""int j = _local_j + _local_j_start;\n""if (j <= (gp1 - 1) - 1) {\n""{""    for (k = 1;""        k < gp2 - 1;""        k++)""    {""        for (m = 0;""            m < 5;""            m++)""        {""            rhs[((j) * ksize * jsize) + ((k) * ksize) + (m)] = rhs[((j) * ksize * jsize) + ((k) * ksize) + (m)] - dssp * (5.0 * u[((j) * uksize * ujsize) + ((k) * uksize) + (m)] - 4.0 * u[((j) * uksize * ujsize) + ((k) * uksize) + (m)] + u[((j) * uksize * ujsize) + ((k) * uksize) + (m)]);""        }""    }""}\n""}\n""}\n""\n";

/* WORKER_FUNCTION 248767 _Function_248767 */
void _Function_248767()
{
    /* Declare Variables */
    int j;
    int gp1;
    int k;
    int gp2;
    int m;
    int i;
    float dssp;
    int uisize;
    int ujsize;
    int uksize;
    int isize;
    int jsize;
    int ksize;
    float ****rhs;
    float ****u;
    /* Declare Generated Variables */
    int _local_data_start, _local_data_end;
    int _local_j_start, _local_j_end;
    int _loop_step;
    int _loop_size;
    int _local_j = 0;
    size_t _work_item_num, _work_group_item_num, _global_item_num;
    cl_kernel _kernel;
    long long _id_rhs;
    cl_mem _cl_mem_rhs = 0;
    long long _id_u;
    cl_mem _cl_mem_u = 0;
    /* Boardcast Scalar Value */
    _GfnEnqueueBoardcastScalar(&gp1, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&gp2, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&i, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&dssp, _GFN_TYPE_FLOAT());
    _GfnEnqueueBoardcastScalar(&uisize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ujsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&uksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&isize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&jsize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&ksize, _GFN_TYPE_INT());
    _GfnEnqueueBoardcastScalar(&_id_rhs, _GFN_TYPE_LONG_LONG_INT());
    _GfnEnqueueBoardcastScalar(&_id_u, _GFN_TYPE_LONG_LONG_INT());
    _GfnFinishBoardcastScalar();
    /* Allocate Array Memory */
    _GfnMalloc4D((void *****) &rhs, &_cl_mem_rhs, _id_rhs, _GFN_TYPE_FLOAT(), isize, jsize, ksize, 5, _GFN_MEM_READ_WRITE(), 1, 1);
    _GfnMalloc4D((void *****) &u, &_cl_mem_u, _id_u, _GFN_TYPE_FLOAT(), uisize, ujsize, uksize, 5, _GFN_MEM_READ_ONLY(), 1, 1);
    /* Initialize Generated Variables */
    _local_data_start = _GfnCalcLocalDataStart(0, (isize) - 1);
    _local_data_end = _GfnCalcLocalDataEnd(0, (isize) - 1);
    _local_j_start = _GfnCalcLocalLoopStart(_local_data_start, 1, 1);
    _local_j_end = _GfnCalcLocalLoopEnd(_local_data_end, (gp1 - 1) - 1);
    _loop_size = _CalcLoopSize(1, (gp1 - 1) - 1, 1);
    _loop_step = 1;
    _work_item_num = _CalcSubSize(_loop_size, _gfn_num_proc, _gfn_rank, 1);
    _work_group_item_num = 64;
    _global_item_num = _GfnCalcGlobalItemNum(_work_item_num, _work_group_item_num);
    /* Allocate Reduce Scalar Variables */
    /* Distribute Array Memory */
    _GfnEnqueueScatterND((void *) rhs[0][0][0], _cl_mem_rhs, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 1, (gp1 - 1) - 1, 1, 0, 0, 1, 1, 4, 0, isize, jsize, ksize, 5);
    _GfnEnqueueScatterND((void *) u[0][0][0], _cl_mem_u, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_ONLY(), 1, (gp1 - 1) - 1, 1, 0, 0, 1, 1, 4, 0, uisize, ujsize, uksize, 5);
    _GfnFinishDistributeArray();
    /* Compute Workload */
    if (1)
    {
        _kernel = _GfnCreateKernel("_kernel_248767");
        _GfnSetKernelArg(_kernel, 0, sizeof(int), (void *) &gp1);
        _GfnSetKernelArg(_kernel, 1, sizeof(int), (void *) &gp2);
        _GfnSetKernelArg(_kernel, 2, sizeof(int), (void *) &i);
        _GfnSetKernelArg(_kernel, 3, sizeof(float), (void *) &dssp);
        _GfnSetKernelArg(_kernel, 4, sizeof(int), (void *) &uisize);
        _GfnSetKernelArg(_kernel, 5, sizeof(int), (void *) &ujsize);
        _GfnSetKernelArg(_kernel, 6, sizeof(int), (void *) &uksize);
        _GfnSetKernelArg(_kernel, 7, sizeof(int), (void *) &isize);
        _GfnSetKernelArg(_kernel, 8, sizeof(int), (void *) &jsize);
        _GfnSetKernelArg(_kernel, 9, sizeof(int), (void *) &ksize);
        _GfnSetKernelArg(_kernel, 10, sizeof(cl_mem), (void *) &_cl_mem_rhs);
        _GfnSetKernelArg(_kernel, 11, sizeof(cl_mem), (void *) &_cl_mem_u);
        _GfnSetKernelArg(_kernel, 12, sizeof(int), (void *) &_local_j_start);
        _GfnSetKernelArg(_kernel, 13, sizeof(int), (void *) &_local_j_end);
        _GfnSetKernelArg(_kernel, 14, sizeof(int), (void *) &_loop_step);
        _GfnLaunchKernel(_kernel, &_global_item_num, &_work_group_item_num);
        _GfnClearKernel(_kernel);
    }
    else
    {
        for (j = _local_j_start;
            j <= _local_j_end;
            j++ , _local_j += _loop_step)
        {
            for (k = 1;
                k < gp2 - 1;
                k++)
            {
                for (m = 0;
                    m < 5;
                    m++)
                {
                    rhs[i][j][k][m] = rhs[i][j][k][m] - dssp * (5.0 * u[i][j][k][m] - 4.0 * u[i + 1][j][k][m] + u[i + 2][j][k][m]);
                }
            }
        }
    }
    /* Gather Array Memory */
    _GfnEnqueueGatherND((void *) rhs[0][0][0], _cl_mem_rhs, _GFN_TYPE_FLOAT(), _GFN_MEM_READ_WRITE(), 1, (gp1 - 1) - 1, 1, 0, 0, 1, 1, 4, 0, isize, jsize, ksize, 5);
    _GfnFinishGatherArray();
    /* Reduce Scalar Value */
    _GfnFinishReduceScalar();
    /* Deallocate Array Memory */
    _GfnFree(_id_rhs, 1, 1);
    _GfnFree(_id_u, 1, 1);
}

