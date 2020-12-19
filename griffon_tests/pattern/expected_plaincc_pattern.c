int main(int argc, char *argv[])
{
    int i, A[300], B[300] = {}, u = 99, it = - 2;
    for (i = 0;
        i < 300;
        i++)
    {
        B[i] = i;
    }
    A[0] = B[0];
    A[299] = B[299];
    /* Send call function message */
    _SendCallFuncMsg(55325);
    _SendConstInputMsg((long long) &(A[0]));
    _SendConstInputMsg((long long) &(B[0]));
    _SendInputNDMsg(&(B[0]), _GFN_TYPE_INT(), 1, (299) - 1, 1, 0, 1, 1, 2, 300, - 1, 1);
    _RecvOutputNDMsg(&(A[0]), _GFN_TYPE_INT(), 1, (299) - 1, 1, 0, 0, 1, 0, 300);
    for (i = 0;
        i < 300;
        i++)
    {
        if (A[i] != B[i])
        {
            printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, B[i], A[i]);
            ((0) ? (void) (0) : __assert_fail("0", "pattern.c", 27, __PRETTY_FUNCTION__));
        }
    }
    printf("TEST PASS!\n");
    return 0;
}
