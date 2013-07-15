long long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, ((void *) 0));
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
double integrate(int n)
{
    double sum, h, x;
    int i;
    h = 1.0 / (double) n;
    /* Send call function message */
    _SendCallFuncMsg(1);
    _SendInputMsg((void *) &n, sizeof(int));
    _SendInputMsg((void *) &h, sizeof(double));
    _RecvOutputMsg((void *) &sum, sizeof(double));
    return sum * h;
}
int main(int argc, char *argv[])
{
    int n, ite, i, elapse;
    double PI25DT = 3.14159265358;
    double pi;
    long long time0, time1;
    n = 2000000;
    ite = 10;
    if (argc > 1)
        n = atoi(argv[1]);
    if (argc > 2)
        ite = atoi(argv[2]);
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    pi = integrate(n);
    time0 = get_time();
    for (i = 0;
        i < ite;
        i++)
        pi = integrate(n);
    time1 = get_time();
    printf("last pi=%.16f, error=%.16f\n", pi, fabs(pi - PI25DT));
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    printf("TEST 01 - Numerical Integration PI\n");
    printf("\tProblem size N = %d\n", n);
    printf("\tRunning iteration = %d\n", ite);
    printf("\tAverage time = %f sec.\n", ((float) (time1 - time0) / 1000000) / ite);
    return 0;
}
