long long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, ((void *) 0));
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
void init(int n, float **A, float **B)
{
    int i, j;
    srand(time(((void *) 0)));
    for (i = 0;
        i < n;
        i++)
    {
        for (j = 0;
            j < n;
            j++)
        {
            A[i][j] = (rand() % 100) / 100.f;
            B[i][j] = (rand() % 100) / 100.f;
        }
    }
}
void matmul_kernel(int n, float **A, float **B, float **C)
{
    int tid;
    int nsquare = n * n;
    int i, j, k;
    /* Send call function message */
    _SendCallFuncMsg(768447);
    _SendInputMsg((void *) &nsquare, sizeof(int));
    _SendInputMsg((void *) &n, sizeof(int));
    _SendInputMsg((void *) &i, sizeof(int));
    _SendInputMsg((void *) &j, sizeof(int));
    _SendConstInputMsg((long long) &(B[0][0]));
    _SendConstInputMsg((long long) &(A[0][0]));
    _SendConstInputMsg((long long) &(C[0][0]));
    _SendInputMsg((void *) B[0], (sizeof(float) * ((n) * (n))));
    _SendInputNDMsg(&(A[0][0]), _GFN_TYPE_FLOAT(), 0, (nsquare) - 1, 1, 0, 0, 2, 0, n, n);
    _RecvOutputNDMsg(&(C[0][0]), _GFN_TYPE_FLOAT(), 0, (nsquare) - 1, 1, 0, 0, 2, 0, n, n);
}
int main(int argc, char *argv[])
{
    int ite, i, j, k, tid;
    int n, pass = 1;
    float **A, **B, **C, sum;
    long long time0, time1;
    n = 1000;
    ite = 10;
    if (argc > 1)
        n = atoi(argv[1]);
    if (argc > 2)
        ite = atoi(argv[2]);
    A = (float **) malloc(n * sizeof(float *));
    A[0] = (float *) malloc(n * n * sizeof(float));
    B = (float **) malloc(n * sizeof(float *));
    B[0] = (float *) malloc(n * n * sizeof(float));
    C = (float **) malloc(n * sizeof(float *));
    C[0] = (float *) malloc(n * n * sizeof(float));
    for (i = 1;
        i < n;
        i++)
    {
        A[i] = A[i - 1] + n;
        B[i] = B[i - 1] + n;
        C[i] = C[i - 1] + n;
    }
    init(n, A, B);
    matmul_kernel(n, A, B, C);
    time0 = get_time();
    for (i = 0;
        i < ite;
        i++)
        matmul_kernel(n, A, B, C);
    time1 = get_time();
    for (i = 0;
        i < n;
        i++)
    {
        for (j = 0;
            j < n;
            j++)
        {
            sum = 0.f;
            for (k = 0;
                k < n;
                k++)
            {
                sum += A[i][k] * B[k][j];
            }
            if (fabs(sum - C[i][j]) > 0.1)
            {
                printf("Error at [%d][%d]\n", i, j);
                printf("C[%d][%d] is %.5f but expected value is %.5f\n", i, j, C[i][j], sum);
                pass = 0;
                break;
            }
        }
        if (!pass)
            break;
    }
    free(A[0]);
    free(A);
    free(B[0]);
    free(B);
    free(C[0]);
    free(C);
    printf("TEST 02 - Matrix Matrix Multiplication\n");
    printf("\tTest result = ");
    if (pass)
        printf("SUCCESSFUL\n");
    else
        printf("FAILURE\n");
    printf("\tProblem size = %d x %d\n", n, n);
    printf("\tRunning iteration = %d\n", ite);
    printf("\tAverage time = %f sec.\n", ((float) (time1 - time0) / 1000000) / ite);
    return 0;
}
