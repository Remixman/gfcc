#include <stdio.h>
#include <stdlib.h>

int main() {
	int i, j, m, n;
	int it, iterator = 10;
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
	sol_f = fopen("solution.txt", "w");
	
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
			fprintf(sol_f, "%2.2f ", matrix[i][j]);
		}
		fprintf(sol_f, "\n");
	}
	
	fclose(matrix_f);
	fclose(sol_f);

	return 0;
}
