int main()
{
    int i, j, sum;
    int A[1000], B[1000];
    int cpuA[1000];
    int C[7];
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    for (i = 0;
        i < 1000;
        ++i)
    {
        B[i] = i + 4;
    }
    C[0] = C[1] = C[2] = C[3] = 7;
    C[4] = C[5] = C[6] = 8;
    /* Send call function message */
    _SendCallFuncMsg(84699);
    _SendConstInputMsg((long long) &(B[0]));
    _SendConstInputMsg((long long) &(A[0]));
    _SendConstInputMsg((long long) &(C[0]));
    _SendInputNDMsg(&(B[0]), _GFN_TYPE_INT(), 0, (1000) - 1, 1, 0, 0, 1, 0, 1000);
    _SendInputMsg((void *) C, (sizeof(int) * ((7))));
    _RecvOutputNDMsg(&(A[0]), _GFN_TYPE_INT(), 0, (1000) - 1, 1, 0, 0, 1, 0, 1000);
    for (i = 0;
        i < 1000;
        ++i)
    {
        cpuA[i] = B[i];
        for (j = 1;
            j < 4;
            ++j)
        {
            cpuA[i] += C[j] + C[j + 3];
        }
    }
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    for (i = 0;
        i < 1000;
        ++i)
    {
        if (cpuA[i] != A[i])
        {
            printf("Fail at i = %d , cpuA[i] = %d but A[i] = %d\n", i, cpuA[i], A[i]);
            ((0) ? (void) (0) : __assert_fail("0", "data_dist.c", 46, __PRETTY_FUNCTION__));
        }
    }
    printf("TEST PASS!\n");
    return 0;
}
