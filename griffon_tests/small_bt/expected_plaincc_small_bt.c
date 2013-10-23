static int grid_points[3];
static double us[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1];
static double vs[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1];
static double ws[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1];
static double qs[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1];
static double rho_i[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1];
static double square[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1];
static double forcing[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1][5 + 1];
static double u[(10 + 1) / 2 * 2 + 1][(10 + 1) / 2 * 2 + 1][(10 + 1) / 2 * 2 + 1][5];
static double rhs[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1][5];
static double lhs[10 / 2 * 2 + 1][10 / 2 * 2 + 1][10 / 2 * 2 + 1][3][5][5];
int func(int x)
{
    return x * x;
}
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
    _SendCallFuncMsg(1);
    _SendInputMsg((void *) &gp0, sizeof(int));
    _SendInputMsg((void *) &gp1, sizeof(int));
    _SendInputMsg((void *) &gp2, sizeof(int));
    _SendInputMsg((void *) &ksize, sizeof(int));
    _SendInputMsg((void *) &jsize, sizeof(int));
    _SendInputMsg((void *) &isize, sizeof(int));
    _SendConstInputMsg((long long) &us);
    _RecvOutputNDMsg(&(us[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    /* Send call function message */
    _SendCallFuncMsg(2);
    _SendInputMsg((void *) &gp0, sizeof(int));
    _SendInputMsg((void *) &gp1, sizeof(int));
    _SendInputMsg((void *) &gp2, sizeof(int));
    _SendInputMsg((void *) &uksize, sizeof(int));
    _SendInputMsg((void *) &ujsize, sizeof(int));
    _SendInputMsg((void *) &uisize, sizeof(int));
    _SendInputMsg((void *) &ksize, sizeof(int));
    _SendInputMsg((void *) &jsize, sizeof(int));
    _SendInputMsg((void *) &isize, sizeof(int));
    _SendConstInputMsg((long long) &qs);
    _SendConstInputMsg((long long) &u);
    _SendConstInputMsg((long long) &rho_i);
    _SendConstInputMsg((long long) &us);
    _SendConstInputMsg((long long) &vs);
    _SendConstInputMsg((long long) &ws);
    _SendConstInputMsg((long long) &square);
    _SendInputNDMsg(&(u[0][0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 4, 0, 5, uksize, ujsize, uisize);
    _RecvOutputNDMsg(&(qs[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    _RecvOutputNDMsg(&(rho_i[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    _RecvOutputNDMsg(&(us[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    _RecvOutputNDMsg(&(vs[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    _RecvOutputNDMsg(&(ws[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    _RecvOutputNDMsg(&(square[0][0][0]), _GFN_TYPE_DOUBLE(), 0, (gp0) - 1, 1, 0, 0, 3, 0, ksize, jsize, isize);
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    return 0;
}
