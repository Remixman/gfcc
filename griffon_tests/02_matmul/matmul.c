#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifdef _GFN
#include <gfn.h>
#endif

#define double_rand() (((double)(rand() / (double)RAND_MAX) - 0.5) * 2)

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

void init(int n, double **A, double **B) {
	int i, j;
	double meanA = 0.0f, meanB = 0.0f;
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			A[i][j] = double_rand();
			meanA += A[i][j];
		}
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			B[i][j] = double_rand();
			meanB += B[i][j];
		}
	}
	printf("initial mean = %f\n", (meanA / (n * n) + meanB / (n * n)));
}

void matmul_kernel(int n, double **A, double **B, double **C) {
	int tid;
	int nsquare = n*n;
	int i, j, k;

	#pragma gfn parallel_for copyin(A[0:n{partition}][0:n],B[0:n][0:n]) copyout(C[0:n{partition}][0:n])
	for (tid = 0; tid < nsquare; ++tid) {
		int i = tid / n;
		int j = tid % n;
		int k = 0;
		double tmp = 0.0;
		for (k = 0; k < n; ++k)
			tmp += A[i][k] * B[k][j];
		C[i][j] = tmp;
	}
}

int main(int argc, char *argv[]) {
	int ite, i, j, k, tid;
	int n, pass = 1;
	double **A, **B, **C, sum;
	long long time0, time1;
	double meanC = 0.0f;

	n = 1000;
	ite = 10;

	if (argc > 1) n = atoi(argv[1]);
	if (argc > 2) ite = atoi(argv[2]);
	
	// allocate memory for A, B and C
	A = (double **) malloc(n * sizeof(double*));
	A[0] = (double *) malloc(n * n * sizeof(double));
	B = (double **) malloc(n * sizeof(double*));
	B[0] = (double *) malloc(n * n * sizeof(double));
	C = (double **) malloc(n * sizeof(double*));
	C[0] = (double *) malloc(n * n * sizeof(double));
	for (i = 1; i < n; i++) {
		A[i] = A[i-1] + n;
		B[i] = B[i-1] + n;
		C[i] = C[i-1] + n;
	}
	
	init(n, A, B);
	
	// warm up
	matmul_kernel(n, A, B, C);

	time0 = get_time();
	for (i = 0; i < ite; i++) matmul_kernel(n, A, B, C);
	time1 = get_time();
	
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			meanC += C[i][j];
	printf("final mean = %f\n", meanC / (n * n));
	
	free(A[0]); free(A);
	free(B[0]); free(B);
	free(C[0]); free(C);

	printf("TEST 02 - Matrix Matrix Multiplication\n");
	printf("\tTest result = ");
	if (pass) printf("SUCCESSFUL\n"); else printf("FAILURE\n");
	printf("\tProblem size = %d x %d\n", n, n);
	printf("\tRunning iteration = %d\n", ite);
	printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	
	return 0;
}

