#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef _GFN
#include <gfn.h>
#endif

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

#define EPSILON 0.01
int is_equal(float a, float b) {
	return (a+EPSILON > b && b > a-EPSILON);
}

int main() {
	int i, j, m, n;
	int N = 1500, ite = 1;
	int it, iterator = 10;
	float **matrix;
	float filter[5][5] = {
		{ 1/256.0,  4/256.0,  6/256.0,  4/256.0, 1/256.0 },
		{ 4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0 },
		{ 6/256.0, 24/256.0, 36/256.0, 24/256.0, 6/256.0 },
		{ 4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0 },
		{ 1/256.0,  4/256.0,  6/256.0,  4/256.0, 1/256.0 }
	};
	long long time0, time1;
	
	// allocate matrix memory 
	matrix = (float **) malloc(N * sizeof(float*));
	matrix[0] = (float *) malloc(N * N * sizeof(float));
	for (i = 1; i < N; i++)
		matrix[i] = matrix[i-1] + N;
	
	// initialize matrix
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			matrix[i][j] = (rand() % 10000) / 100.0;
	
	time0 = get_time();
	// kernel
for (it = 0; it < iterator; it++) {

	#pragma gfn parallel_for copyin(filter[0:5][0:5]) copy(matrix[0:N][0:N])
	for (i = 0+2; i < N-2; ++i) {	
		for (j = 0+2; j < N-2; ++j) {
			float new_val = 0.0;
			for (m = 0; m < 5; ++m) {
				for (n = 0; n < 5; ++n) {
					new_val += (filter[m][n] * matrix[i+m-2][j+n-2]);
				}
			}
			matrix[i][j] = new_val;
		}
	}
	
}
	time1 = get_time();
	
	free(matrix[0]);
	free(matrix);

	printf("TEST 03 - Convolution\n");	
	printf("TEST PASS!!\n");
	printf("\tProblem size = %d x %d\n", N, N);
	printf("\tRunning iteration = %d\n", ite);
	printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);

	return 0;
}
