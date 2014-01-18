#include <stdio.h>
#include <assert.h>
#ifdef _GFN
#include <gfn.h>
#endif

int main(int argc, char *argv[]) {
  int i, j, C[50][50], A[50][50], B[50][50];
  
  for (i=0;i<50;i++)
  	for (j=0;j<50;j++)
  		A[i][j] = B[i][j] = i*50+j;
  
  #pragma gfn parallel_for \
  	copyin(A[0:50{partition}][0:50]) \
  	copyin(B[0:50][0:50]) \
  	copyout(C[0:50{partition}][0:50])
  for (i=0;i<50;++i) {
  	#pragma gfn loop
  	for (j=0;j<50;++j) {
  		C[i][j] = A[i][j] + B[i][j];
  	}
  }
  
  for (i=0;i<50;++i)
  	for (j=0;j<50;++j)
  		assert(C[i][j] == (A[i][j]+B[i][j]));
  
  return 0;
}
