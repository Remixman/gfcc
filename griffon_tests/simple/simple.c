#include <stdio.h>
#include <assert.h>
#include <gfn.h>

int main(int argc, char *argv[]) {
  int i, A[300], B[300] = {}, u = 99, it=-2;
  
  #pragma gfn start
  
  for (i=0;i<300;i++) {
  	B[i] = i;
  }
  
  #pragma gfn parallel_for size(A,B,C:300) size(D:4) input(B,b) output(A) reduction(+:s)
  for (i=0;i<300;i++) {
    A[i] = B[i];                                         
    #pragma gfn barrier
    A[i] *= -1;
  }
  
  #pragma gfn finish
  
  // Test result
  for (i=0;i<300;i++) {
  	if (A[i] != (-1*B[i])) {
  	  printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, -1*B[i], A[i]);
      assert(0);
  	}
  }
  printf("TEST PASS!\n");
  
  return 0;
}
