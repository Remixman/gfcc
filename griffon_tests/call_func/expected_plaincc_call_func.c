int b(int x)
{
    return x * x;
}
int c(int x)
{
    return x + x + 1;
}
int a(int x, int y)
{
    return b(x) + c(y);
}
int main(int argc, char *argv[])
{
    int i, A[500], B[500];
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    for (i = 0;
        i < 500;
        i++)
    {
        B[i] = i;
    }
    /* Send call send function message */
    _SendCallFuncMsg(145409);
    _SendInputMsg((void *) B, (sizeof(int) * ((500))));
    _GfnLockTransfer((void *) B);
    {
        /* Send call function message */
        _SendCallFuncMsg(133237);
        _SendConstInputMsg((long long) &A);
        _SendConstInputMsg((long long) &B);
        _SendInputMsg((void *) B, (sizeof(int) * ((500))));
        _RecvOutputNDMsg(&(A[0]), _GFN_TYPE_INT(), 0, (500) - 1, 1, 0, 0, 1, 0, 500);
    }
    /* Send call recieve function message */
    _SendCallFuncMsg(530979);
    _GfnUnlockTransfer((void *) A);
    _RecvOutputMsg((void *) A, (sizeof(int) * ((500))));
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    for (i = 0;
        i < 500;
        i++)
    {
        if (A[i] != a(B[i], 8))
        {
            printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, a(B[i], 8), A[i]);
            ((0) ? (void) (0) : __assert_fail("0", "call_func.c", 46, __PRETTY_FUNCTION__));
        }
    }
    printf("TEST PASS!\n");
    return 0;
}
