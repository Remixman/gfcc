#include <stdio.h>
#include <assert.h>
#include <gfn.h>

int main() {
	
	int i, j, sum;
	int A[1000], B[1000];
	int cpuA[1000];
	int C[7];
	
	#pragma gfn start
	
	// Initialize B and C
	for (i = 0; i < 1000; ++i) {
		B[i] = i+4;
	}
	C[0]=C[1]=C[2]=C[3] = 7;
	C[4]=C[5]=C[6] = 8;
	
	// Compute by GPU Cluster
	#pragma gfn parallel_for input(B,C) output(A) size(A,B:1000) size(C:7)
	for (i = 0; i < 1000; ++i) {
		A[i] = B[i];
		for (j = 1; j < 4; ++j) {
			A[i] += C[j] + C[j+3];
		}
	}
	
	// Computer by CPU
	for (i = 0; i < 1000; ++i) {
		cpuA[i] = B[i];
		for (j = 1; j < 4; ++j) {
			cpuA[i] += C[j] + C[j+3];
		}
	}
	
	#pragma gfn finish

	
	// Test result
	for (i = 0; i < 1000; ++i) {
		if (cpuA[i] != A[i]) {
			printf("Fail at i = %d , cpuA[i] = %d but A[i] = %d\n", i, cpuA[i], A[i]);
			assert(0);
		}
	}
	printf("TEST PASS!\n");

	return 0;
}

