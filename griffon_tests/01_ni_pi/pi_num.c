#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef _GRIFFON
#include <gfn.h>
#endif

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

double integrate(int n) {
	double sum, h, x;
	int i;
	h = 1.0 / (double) n;
	#pragma gfn parallel_for reduction(+:sum)
	for (i = 1; i <= n; i++) {
		x = h*((double)i - 0.5);
		sum += 4.0 / (1.0 + x*x);
	}
	return sum * h;
}

int main(int argc, char *argv[]) {
	int n, ite, i, elapse;
	double PI25DT = 3.14159265358;
	double pi;
	long long time0, time1;

	n = 2000000;
	ite = 10;

	if (argc > 1) n = atoi(argv[1]);
	if (argc > 2) ite = atoi(argv[2]);

	// warm up
	pi = integrate(n);

	time0 = get_time();
	for(i = 0; i < ite; i++) pi = integrate(n);
	time1 = get_time();
		
	printf("last pi=%.16f, error=%.16f\n", pi, fabs(pi - PI25DT));

	printf("TEST 01 - Numerical Integration PI\n");
	printf("\tProblem size N = %d\n", n);
	printf("\tRunning iteration = %d\n", ite);
	printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	
	return 0;
}

