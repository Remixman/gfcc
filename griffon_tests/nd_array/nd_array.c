#include <stdio.h>
#include <assert.h>
#include <gfn.h>

int main() {
	
	int i, j, k;
	// TODO: increase to 5 dimensions
	// TODO: nested parallel
	double A[4][20][100];
	
	#pragma gfn start
	
	#pragma gfn parallel_for output(A) size(A:4,20,100)
	for (i = 0; i < 4; i++)
		for (j = 0; j < 20; j++)
			for (k = 0; k < 100; k++)
				A[i][j][k] = 123;

	
	#pragma gfn finish

	
	// Test result
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 20; j++) {
			for (k = 0; k < 100; k++) {
				if (A[i][j][k] != 123) {
					printf("Fail at i = %d , j = %d , k = %d, A[i][j][k] = %lf\n", i, j, k, A[i][j][k]);
					assert(0);
				}
			}
		}
	}
	printf("TEST PASS!\n");

	return 0;
}

