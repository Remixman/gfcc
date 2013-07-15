int main(int argc, char *argv[])
{
    int i, *A, n = 5000;
    int expected_result = (n * (n + 1)) / 2;
    int error = 0;
    int s1 = 0;
    float s2 = 0;
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    /* Send call function message */
    _SendCallFuncMsg(1);
    _SendInputMsg((void *) &n, sizeof(int));
    _RecvOutputMsg((void *) &s1, sizeof(int));
    _RecvOutputMsg((void *) &s2, sizeof(float));
    if (expected_result != s1)
    {
        printf("s1 result error!! (Expected is [%d] but s1 is [%d])\n", expected_result, s1);
        error = 1;
    }
    if (expected_result != s2)
    {
        printf("s2 result error!! (Expected is [%d] but s2 is [%d])\n", expected_result, (int) s2);
        error = 1;
    }
    if (!error)
    {
        printf("TEST PASS!\n");
    }
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    return 0;
}
