int main(int argc, char *argv[])
{
    int i, j, C[50][50], A[50][50], B[50][50];
    for (i = 0;
        i < 50;
        i++)
        for (j = 0;
            j < 50;
            j++)
            A[i][j] = B[i][j] = i * 50 + j;
    /* Send call function message */
    _SendCallFuncMsg(863273);
    _SendConstInputMsg((long long) &(C[0][0]));
    _SendConstInputMsg((long long) &(A[0][0]));
    _SendConstInputMsg((long long) &(B[0][0]));
    _SendInputNDMsg(&(A[0][0]), _GFN_TYPE_INT(), 0, (50) - 1, 1, 0, 0, 2, 0, 50, 50);
    _SendInputMsg((void *) B[0], (sizeof(int) * ((50) * (50))));
    _RecvOutputNDMsg(&(C[0][0]), _GFN_TYPE_INT(), 0, (50) - 1, 1, 0, 0, 2, 0, 50, 50);
    for (i = 0;
        i < 50;
        ++i)
        for (j = 0;
            j < 50;
            ++j)
            if (C[i][j] != (A[i][j] + B[i][j]))
            {
                printf("Error at i=%d,j=%d Expected value is %d, actual value is %d\n", i, j, A[i][j] + B[i][j], C[i][j]);
                return - 1;
            }
    printf("SUCCESS\n");
    return 0;
}
