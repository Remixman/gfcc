#include <stdio.h>
#include <assert.h>
#include <gfn.h>

int main(int argc, char *argv[]) {
  int i, A[300], B[300] = {}, u = 99, it=-2;
  
  #pragma gfn start
  
  for (i=0;i<300;i++) {
  	B[i] = i;
  }
  
  A[0] = B[0];
  A[299] = B[299];
  
  #pragma gfn parallel_for input(B[300]) output(A[300]) \
  	in_pattern(B:[-1,1]) if(1,0)
  for (i=1;i<299;i++) {
    A[i] = (B[i-1] + B[i] + B[i+1]) / 3;
  }
  
  #pragma gfn finish
  
  // Test result
  for (i=0;i<300;i++) {
  	if (A[i] != B[i]) {
  	  printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, B[i], A[i]);
      assert(0);
  	}
  }
  printf("TEST PASS!\n");
  
  return 0;
}

