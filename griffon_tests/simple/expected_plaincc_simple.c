int main(int argc, char *argv[])
{
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
    _SendInputNDMsg(&(B[0]), _GFN_TYPE_INT(), 0, (300) - 1, 1, 0, 0, 1, 0, 300);
    _RecvOutputNDMsg(&(A[0]), _GFN_TYPE_INT(), 0, (300) - 1, 1, 0, 0, 1, 0, 300);
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
            ((0) ? (void) (0) : __assert_fail("0", "simple.c", 26, __PRETTY_FUNCTION__));
        }
    }
    printf("TEST PASS!\n");
    return 0;
}
