long long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, ((void *) 0));
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
void init(int N, float **mat)
{
    int i, j;
    for (i = 0;
        i < N;
        i++)
        for (j = 0;
            j < N;
            j++)
            mat[i][j] = (rand() % 100) / 100.f;
}
void copy_matrix(int N, float **src, float **dsc)
{
    int i, j;
    for (i = 0;
        i < N;
        i++)
        for (j = 0;
            j < N;
            j++)
            dsc[i][j] = src[i][j];
}
int is_equal(float a, float b)
{
    return (a + 0.01 > b && b > a - 0.01);
}
void convolution_kernel(int N, int iterator, float **matrix, int filterN, float **filter)
{
    int i, j, m, n, tid;
    int Nsquare = N * N;
    int it;
    /* Send call send function message */
    _SendCallFuncMsg(355380);
    _SendConstInputMsg((long long) &(filter[0][0]));
    _SendConstInputMsg((long long) &(matrix[0][0]));
    _SendInputMsg((void *) &N, sizeof(int));
    _SendInputMsg((void *) filter[0], (sizeof(float) * ((5) * (5))));
    _SendInputNDMsg(&(matrix[0][0]), _GFN_TYPE_FLOAT(), 0, 0, 0, 0, 0, 2, 0, N, N);
    _GfnLockTransfer((long long) &(filter[0][0]));
    _GfnLockTransfer((long long) &(matrix[0][0]));
    {
        for (it = 0;
            it < iterator;
            it++)
        {
            /* Send call function message */
            _SendCallFuncMsg(667325);
            _SendInputMsg((void *) &Nsquare, sizeof(int));
            _SendInputMsg((void *) &N, sizeof(int));
            _SendInputMsg((void *) &filterN, sizeof(int));
            _SendConstInputMsg((long long) &(filter[0][0]));
            _SendConstInputMsg((long long) &(matrix[0][0]));
            _SendInputMsg((void *) filter[0], (sizeof(float) * ((5) * (5))));
            _SendInputNDMsg(&(matrix[0][0]), _GFN_TYPE_FLOAT(), 0, (Nsquare) - 1, 1, 0, 1, 2, 2, N, N, - 2, 2);
            _RecvOutputNDMsg(&(matrix[0][0]), _GFN_TYPE_FLOAT(), 0, (Nsquare) - 1, 1, 0, 0, 2, 0, N, N);
        }
    }
    /* Send call recieve function message */
    _SendCallFuncMsg(360592);
    _SendInputMsg((void *) &N, sizeof(int));
    _GfnUnlockTransfer((long long) &(filter[0][0]));
    _GfnUnlockTransfer((long long) &(matrix[0][0]));
    _RecvOutputNDMsg(&(matrix[0][0]), _GFN_TYPE_FLOAT(), 0, 0, 0, 0, 0, 2, 0, N, N);
}
int main(int argc, char *argv[])
{
    int i, j, m, n;
    int tid;
    int N = 1500, ite = 1;
    int it, iterator = 10;
    float **matrix, **orig_mat, **filter;
    long long time0, time1;
    int pass = 1;
    N = 1500;
    ite = 3;
    if (argc > 1)
        N = atoi(argv[1]);
    if (argc > 2)
        ite = atoi(argv[2]);
    matrix = (float **) malloc(N * sizeof(float *));
    matrix[0] = (float *) malloc(N * N * sizeof(float));
    for (i = 1;
        i < N;
        i++)
        matrix[i] = matrix[i - 1] + N;
    orig_mat = (float **) malloc(N * sizeof(float *));
    orig_mat[0] = (float *) malloc(N * N * sizeof(float));
    for (i = 1;
        i < N;
        i++)
        orig_mat[i] = orig_mat[i - 1] + N;
    filter = (float **) malloc(5 * sizeof(float *));
    filter[0] = (float *) malloc(5 * 5 * sizeof(float));
    for (i = 1;
        i < 5;
        i++)
        filter[i] = filter[i - 1] + 5;
    filter[0][0] = 1 / 256.0;
    filter[0][1] = 4 / 256.0;
    filter[0][2] = 6 / 256.0;
    filter[0][3] = 4 / 256.0;
    filter[0][4] = 1 / 256.0;
    filter[1][0] = 4 / 256.0;
    filter[1][1] = 16 / 256.0;
    filter[1][2] = 24 / 256.0;
    filter[1][3] = 16 / 256.0;
    filter[1][4] = 4 / 256.0;
    filter[2][0] = 6 / 256.0;
    filter[2][1] = 24 / 256.0;
    filter[2][2] = 36 / 256.0;
    filter[2][3] = 24 / 256.0;
    filter[2][4] = 6 / 256.0;
    filter[3][0] = 4 / 256.0;
    filter[3][1] = 16 / 256.0;
    filter[3][2] = 24 / 256.0;
    filter[3][3] = 16 / 256.0;
    filter[3][4] = 4 / 256.0;
    filter[4][0] = 1 / 256.0;
    filter[4][1] = 4 / 256.0;
    filter[4][2] = 6 / 256.0;
    filter[4][3] = 4 / 256.0;
    filter[4][4] = 1 / 256.0;
    init(N, orig_mat);
    copy_matrix(N, orig_mat, matrix);
    convolution_kernel(N, iterator, matrix, 5, (float **) filter);
    time0 = get_time();
    for (i = 0;
        i < ite;
        i++)
    {
        copy_matrix(N, orig_mat, matrix);
        convolution_kernel(N, iterator, matrix, 5, (float **) filter);
    }
    time1 = get_time();
    for (it = 0;
        it < iterator;
        it++)
    {
        for (i = 0 + 2;
            i < N - 2;
            ++i)
        {
            for (j = 0 + 2;
                j < N - 2;
                ++j)
            {
                float new_val = 0.0;
                for (m = 0;
                    m < 5;
                    ++m)
                {
                    for (n = 0;
                        n < 5;
                        ++n)
                    {
                        new_val += (filter[m][n] * orig_mat[i + m - 2][j + n - 2]);
                    }
                }
                orig_mat[i][j] = new_val;
            }
        }
    }
    for (i = 0;
        i < N;
        ++i)
    {
        for (j = 0;
            j < N;
            ++j)
        {
            if (fabs(orig_mat[i][j] - matrix[i][j]) > 0.1)
            {
                printf("Error at [%d][%d]\n", i, j);
                printf("Actual value is %.5f but expected value is %.5f\n", matrix[i][j], orig_mat[i][j]);
                pass = 0;
                break;
            }
        }
    }
    printf("TEST 03 - Convolution\n");
    printf("\tTest result = ");
    if (pass)
        printf("SUCCESSFUL\n");
    else
        printf("FAILURE\n");
    printf("\tProblem size = %d x %d\n", N, N);
    printf("\tRunning iteration = %d\n", ite);
    printf("\tAverage time = %f sec.\n", ((float) (time1 - time0) / 1000000) / ite);
    free(matrix[0]);
    free(matrix);
    free(orig_mat[0]);
    free(orig_mat);
    free(filter[0]);
    free(filter);
    return 0;
}
