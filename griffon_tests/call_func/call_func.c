#include <stdio.h>
#include <assert.h>
#ifdef _GFN
#include <gfn.h>
#endif

int main(int argc, char *argv[]) {
  int i, j, C[47][47], A[47][47], B[47][47];
  
  for (i=0;i<47;i++)
  	for (j=0;j<47;j++)
  		A[i][j] = B[i][j] = i*47+j;
  
  #pragma gfn parallel_for \
  	copyin(A[0:47{partition}][0:47]) \
  	copyin(B[0:47{partition}][0:47]) \
  	copyout(C[0:47{partition}][0:47])
  for (i=0;i<47;i++) {
  	#pragma gfn loop
  	for (j=0;j<47;j++) {
  		C[i][j] = A[i][j] + B[i][j];
  	}
  }
  
  for (i=0;i<47;i++)
  	for (j=0;j<47;j++)
  		if (C[i][j] != (A[i][j]+B[i][j])) {
  			printf("Error at i=%d,j=%d Expected value is %d, actual value is %d\n", 
  				i, j, A[i][j]+B[i][j], C[i][j]);
  			return -1;
  		}
  
  printf("SUCCESS\n");
  
  return 0;
}
