#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

void init(int N, double **mat) {
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			/* Random from 0.00 - 0.99 */
			mat[i][j] = (rand() % 100) / 100.f;
}

void copy_matrix(int N, double **src, double **dsc) {
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			dsc[i][j] = src[i][j];
}

#define EPSILON 0.01
int is_equal(double a, double b) {
	return (a+EPSILON > b && b > a-EPSILON);
}

void convolution_kernel(int N, int iterator, double **matrix, double **result_matrix)
{
	int i, j, m, n, tid;
	int Nsquare = N*N;
	int it;

	// run kernel
	#pragma gfn data create(result_matrix[0:N][0:N]) \
		copy(matrix[0:N{partition}][0:N])
	{
	for (it = 0; it < iterator; it++) {

	#pragma gfn parallel_for present(result_matrix[0:N][0:N]) \
		pcopyin(matrix[0:N{partition}][0:N]) \
		in_pattern(matrix:[-60,60][-60,60]) \
		private(i, j, m, n)
	for (tid = 0; tid < Nsquare; ++tid) {
		i = tid / N;
		j = tid % N;
		if (i >= 60 && i < N-60 && j >= 60 && j < N-60) {
			double new_val = 0.0;
			for (m = 0; m < 121; ++m) {
				for (n = 0; n < 121; ++n) {
					new_val += matrix[(i+m-60)][(j+n-60)];
				}
			}
			result_matrix[i][j] = new_val / 14641.0;
		} else {
			result_matrix[i][j] = matrix[i][j];
		}
	}

	#pragma gfn parallel_for present(result_matrix[0:N][0:N]) \
		pcopyout(matrix[0:N{partition}][0:N])
	for (tid = 0; tid < Nsquare; ++tid) {
		i = tid / N;
		j = tid % N;
		matrix[i][j] = result_matrix[i][j];
	}

	}

	} /* end pragma acc data */
}


int main(int argc, char *argv[]) {
	int i, j, m, n;
	int tid;
	int N = 1500, ite = 1;
	int it, iterator = 10;
	double **matrix, **orig_mat;
	double **result_matrix, **verify_matrix;
	long long time0, time1;
	int pass = 1;

	N = 1500;
	ite = 1;

	if (argc > 1) N = atoi(argv[1]);
	if (argc > 2) ite = atoi(argv[2]);
	
	// allocate matrix memory 
	matrix = (double **) malloc(N * sizeof(double*));
	matrix[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		matrix[i] = matrix[i-1] + N;
	result_matrix = (double **) malloc(N * sizeof(double*));
	result_matrix[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		result_matrix[i] = result_matrix[i-1] + N;
	orig_mat = (double **) malloc(N * sizeof(double*));
	orig_mat[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		orig_mat[i] = orig_mat[i-1] + N;
	verify_matrix = (double **) malloc(N * sizeof(double*));
	verify_matrix[0] = (double *) malloc(N * N * sizeof(double));
	for (i = 1; i < N; i++)
		verify_matrix[i] = verify_matrix[i-1] + N;

	// initialize matrix
	init(N, orig_mat);
	
	// warm up
	copy_matrix(N, orig_mat, matrix);
	convolution_kernel(N, iterator, matrix, result_matrix);

	
	for (i = 0; i < ite; i++) {
		copy_matrix(N, orig_mat, matrix);
		time0 = get_time();
		convolution_kernel(N, iterator, matrix, result_matrix);
		time1 = get_time();
	}

	// assert and show result
	for (it = 0; it < iterator; it++) {
	// apply filter
	for (tid = 0; tid < N*N; ++tid) {
		i = tid / N;
		j = tid % N;
		if (i >= 60 && i < N-60 && j >= 60 && j < N-60) {
			double new_val = 0.0;
			for (m = 0; m < 121; ++m) {	
				for (n = 0; n < 121; ++n) {	
					new_val += orig_mat[i+m-60][j+n-60];
				}
			}
			verify_matrix[i][j] = new_val / 14641.0;
		} else {
			verify_matrix[i][j] = orig_mat[i][j];
		}
	}

	// copy result to original matrix
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (i >= 60 && i < N-60 && j >= 60 && j < N-60) {
				orig_mat[i][j] = verify_matrix[i][j];
			}
		}
	}
	}

	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			if (fabs(orig_mat[i][j]-matrix[i][j]) > 0.1) {
				printf("Error at [%d][%d]\n", i, j);
				printf("Actual value is %.5f but expected value is %.5f\n",
					matrix[i][j], orig_mat[i][j]);
					pass = 0;
				break;
			}
		}
	}

	printf("TEST 03 - Convolution\n");
	printf("\tTest result = ");
	if (pass) printf("SUCCESSFUL\n"); else printf("FAILURE\n");
	printf("\tProblem size = %d x %d\n", N, N);
	printf("\tRunning iteration = %d\n", ite);
	printf("\tAverage time = %f sec.\n", ((float)(time1-time0)/1000000)/ite);
	
	free(matrix[0]);
	free(matrix);
	free(result_matrix[0]);
	free(result_matrix);
	free(orig_mat[0]);
	free(orig_mat);
	free(verify_matrix[0]);
	free(verify_matrix);

	return 0;
}
