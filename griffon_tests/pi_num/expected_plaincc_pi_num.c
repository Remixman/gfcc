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
    int n, elapse;
    double PI25DT = 3.14159265358;
    double pi;
    /* Initialize IPC to worker */
    _OpenMasterMsgQueue();
    while (1)
    {
        printf("Enter # intervals: ");
        scanf("%d", &n);
        if (n == 0)
            break;
        pi = integrate(n);
        printf("pi=%.16f, error=%.16f\n", pi, fabs(pi - PI25DT));
    }
    /* Close IPC to worker */
    _SendCallFuncMsg(0);
    _CloseMasterMsgQueue();
    return 0;
}
