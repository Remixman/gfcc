static int grid_points[3];
static float us[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1];
static float vs[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1];
static float ws[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1];
static float qs[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1];
static float rho_i[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1];
static float square[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1];
static float forcing[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1][5 + 1];
static float u[(3 + 1) / 2 * 2 + 1][(3 + 1) / 2 * 2 + 1][(3 + 1) / 2 * 2 + 1][5];
static float rhs[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1][5];
static float lhs[3 / 2 * 2 + 1][3 / 2 * 2 + 1][3 / 2 * 2 + 1][3][5][5];
int main()
{
    int i, j, k, m, n;
    float rho_inv, uijk, up1, um1, vijk, vp1, vm1, wijk, wp1, wm1;
    float dssp = 5.67;
    int gp0, gp1, gp2;
    int isize, jsize, ksize;
    int uisize, ujsize, uksize;
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    gp0 = grid_points[0] = isize = 3 / 2 * 2 + 1;
    gp1 = grid_points[1] = jsize = 3 / 2 * 2 + 1;
    gp2 = grid_points[2] = ksize = 3 / 2 * 2 + 1;
    uisize = (3 + 1) / 2 * 2 + 1;
    ujsize = (3 + 1) / 2 * 2 + 1;
    uksize = (3 + 1) / 2 * 2 + 1;
    /* Send call function message */
    _SendCallFuncMsg(837377);
    _SendInputMsg((void *) &gp0, sizeof(int));
    _SendInputMsg((void *) &gp1, sizeof(int));
    _SendInputMsg((void *) &gp2, sizeof(int));
    _SendInputMsg((void *) &isize, sizeof(int));
    _SendInputMsg((void *) &jsize, sizeof(int));
    _SendInputMsg((void *) &ksize, sizeof(int));
    _SendConstInputMsg((long long) &(us[0][0][0]));
    _RecvOutputNDMsg(&(us[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    /* Send call function message */
    _SendCallFuncMsg(164222);
    _SendInputMsg((void *) &gp0, sizeof(int));
    _SendInputMsg((void *) &gp1, sizeof(int));
    _SendInputMsg((void *) &gp2, sizeof(int));
    _SendInputMsg((void *) &isize, sizeof(int));
    _SendInputMsg((void *) &jsize, sizeof(int));
    _SendInputMsg((void *) &ksize, sizeof(int));
    _SendConstInputMsg((long long) &(lhs[0][0][0][0][0][0]));
    _RecvOutputNDMsg(&(lhs[0][0][0][0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 6, 0, isize, jsize, ksize, 3, 5, 5);
    /* Send call function message */
    _SendCallFuncMsg(982620);
    _SendInputMsg((void *) &gp0, sizeof(int));
    _SendInputMsg((void *) &gp1, sizeof(int));
    _SendInputMsg((void *) &gp2, sizeof(int));
    _SendInputMsg((void *) &uisize, sizeof(int));
    _SendInputMsg((void *) &ujsize, sizeof(int));
    _SendInputMsg((void *) &uksize, sizeof(int));
    _SendInputMsg((void *) &isize, sizeof(int));
    _SendInputMsg((void *) &jsize, sizeof(int));
    _SendInputMsg((void *) &ksize, sizeof(int));
    _SendConstInputMsg((long long) &(qs[0][0][0]));
    _SendConstInputMsg((long long) &(u[0][0][0][0]));
    _SendConstInputMsg((long long) &(rho_i[0][0][0]));
    _SendConstInputMsg((long long) &(us[0][0][0]));
    _SendConstInputMsg((long long) &(vs[0][0][0]));
    _SendConstInputMsg((long long) &(ws[0][0][0]));
    _SendConstInputMsg((long long) &(square[0][0][0]));
    _SendInputNDMsg(&(u[0][0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 4, 0, uisize, ujsize, uksize, 5);
    _RecvOutputNDMsg(&(qs[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    _RecvOutputNDMsg(&(rho_i[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    _RecvOutputNDMsg(&(us[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    _RecvOutputNDMsg(&(vs[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    _RecvOutputNDMsg(&(ws[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    _RecvOutputNDMsg(&(square[0][0][0]), _GFN_TYPE_FLOAT(), 0, (gp0) - 1, 1, 0, 0, 3, 0, isize, jsize, ksize);
    i = 1;
    /* Send call function message */
    _SendCallFuncMsg(248767);
    _SendInputMsg((void *) &gp1, sizeof(int));
    _SendInputMsg((void *) &gp2, sizeof(int));
    _SendInputMsg((void *) &i, sizeof(int));
    _SendInputMsg((void *) &dssp, sizeof(float));
    _SendInputMsg((void *) &uisize, sizeof(int));
    _SendInputMsg((void *) &ujsize, sizeof(int));
    _SendInputMsg((void *) &uksize, sizeof(int));
    _SendInputMsg((void *) &isize, sizeof(int));
    _SendInputMsg((void *) &jsize, sizeof(int));
    _SendInputMsg((void *) &ksize, sizeof(int));
    _SendConstInputMsg((long long) &(rhs[0][0][0][0]));
    _SendConstInputMsg((long long) &(u[0][0][0][0]));
    _SendInputNDMsg(&(rhs[0][0][0][0]), _GFN_TYPE_FLOAT(), 1, (gp1 - 1) - 1, 1, 0, 0, 4, 0, isize, jsize, ksize, 5);
    _SendInputNDMsg(&(u[0][0][0][0]), _GFN_TYPE_FLOAT(), 1, (gp1 - 1) - 1, 1, 0, 0, 4, 0, uisize, ujsize, uksize, 5);
    _RecvOutputNDMsg(&(rhs[0][0][0][0]), _GFN_TYPE_FLOAT(), 1, (gp1 - 1) - 1, 1, 0, 0, 4, 0, isize, jsize, ksize, 5);
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    return 0;
}
