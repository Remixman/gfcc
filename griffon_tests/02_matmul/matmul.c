#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gfn.h>

#define N 500

int n = N;
float A[N][N]; 
float B[N][N]; 
float C[N][N];

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

void init() {
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

void matmul_kernel() {
	int i, j, k;
	
	#pragma gfn parallel_for input(A[n][n],B[n][n]) output(C[n][n])
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
	int ite, i;
	long long time0, time1;

	ite = 10;

	if (argc > 1) n = atoi(argv[1]);
	if (argc > 2) ite = atoi(argv[2]);
	
	#pragma gfn start
	
	init();
  
	// warm up
	matmul_kernel();

	time0 = get_time();
	for(i = 0; i < ite; i++) matmul_kernel();
	time1 = get_time();
	
	#pragma gfn finish

	printf("TEST 02 - Matrix Matrix Multiplication\n");
	printf("\tProblem size = %d x %d\n", n, n);
	printf("\tRunning iteration = %d\n", ite);
	printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	
	return 0;
}

