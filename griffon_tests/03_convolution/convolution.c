#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define EPSILON 0.01
int is_equal(float a, float b) {
	return (a+EPSILON > b && b > a-EPSILON);
}

int main() {
	int i, j, m, n;
	int it, iterator = 10;
	float sol_tmp;
	float matrix[1000][1000];
	float filter[5][5] = {
		{ 1/256.0,  4/256.0,  6/256.0,  4/256.0, 1/256.0 },
		{ 4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0 },
		{ 6/256.0, 24/256.0, 36/256.0, 24/256.0, 6/256.0 },
		{ 4/256.0, 16/256.0, 24/256.0, 16/256.0, 4/256.0 },
		{ 1/256.0,  4/256.0,  6/256.0,  4/256.0, 1/256.0 }
	};

	FILE *matrix_f;
	FILE *sol_f;
	
	matrix_f = fopen("matrix.txt", "r");
	sol_f = fopen("solution.txt", "r");
	
	for (i = 0; i < 1000; ++i) {
		for (j = 0; j < 1000; ++j) {
			fscanf(matrix_f, "%f", &(matrix[i][j]));
		}
	}
	
	// kernel
for (it = 0; it < iterator; it++) {

	for (i = 0+2; i < 1000-2; ++i) {	
		for (j = 0+2; j < 1000-2; ++j) {
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
	
	for (i = 0; i < 1000; ++i) {
		for (j = 0; j < 1000; ++j) {
			fscanf(sol_f, "%f ", &sol_tmp);
			//printf("Compare [%d][%d], solution = %2.6f, conv = %2.6f\n", i, j, sol_tmp, matrix[i][j]);
			assert(is_equal(sol_tmp,matrix[i][j]));
		}
	}
	
	fclose(matrix_f);
	fclose(sol_f);
	
	printf("TEST PASS!!\n");

	return 0;
}
