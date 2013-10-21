#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifdef _GFN
#include <gfn.h>
#endif

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

void init(int n, float **A, float **B) {
	int i, j;
	
	srand(time(NULL));
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			/* Random from 0.00 - 0.99 */
			A[i][j] = (rand() % 100) / 100.f;
			B[i][j] = (rand() % 100) / 100.f;
		}
	}
}

void matmul_kernel(int n, float **A, float **B, float **C) {
	int i, j, k;

	#pragma gfn parallel_for copyin(A[0:n{partition}][0:n],B[0:n][0:n]) copyout(C[0:n{partition}][0:n])
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			C[i][j] = 0.f;
			for (k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

int main(int argc, char *argv[]) {
	int ite, i, j, k;
	int n, pass = 1;
	float **A, **B, **C, sum;
	long long time0, time1;

	n = 1000;
	ite = 10;

	if (argc > 1) n = atoi(argv[1]);
	if (argc > 2) ite = atoi(argv[2]);
	
	#pragma gfn start
	
	// allocate memory for A, B and C
	A = (float **) malloc(n * sizeof(float*));
	A[0] = (float *) malloc(n * n * sizeof(float));
	B = (float **) malloc(n * sizeof(float*));
	B[0] = (float *) malloc(n * n * sizeof(float));
	C = (float **) malloc(n * sizeof(float*));
	C[0] = (float *) malloc(n * n * sizeof(float));
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
	
	// assert with sequential solution
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			sum = 0.f;
			for (k = 0; k < n; k++) {
				sum += A[i][k] * B[k][j];
			}
			if (fabs(sum-C[i][j]) > 0.0001) {
				printf("Error at [%d][%d]\n", i, j);
				printf("C[%d][%d] is %.5f but expected value is %.5f\n");
				pass = 0;
				break;
			}
		}
		if (!pass) break;
	}
	
	#pragma gfn finish

	printf("TEST 02 - Matrix Matrix Multiplication\n");
	printf("\tTest result = ");
	if (pass) printf("SUCCESSFUL\n"); else printf("FAILURE\n");
	printf("\tProblem size = %d x %d\n", n, n);
	printf("\tRunning iteration = %d\n", ite);
	printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	
	return 0;
}

