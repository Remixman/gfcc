#include <stdio.h>
#include <assert.h>
#ifdef _GFN
#include <gfn.h>
#endif

#pragma gfn use_in_parallel
int b(int x) {
  return x * x;
}
#pragma gfn use_in_parallel
int c(int x) {
  return x + x + 1;
}
#pragma gfn use_in_parallel
int a(int x, int y) {
  return b(x) + c(y);
}

int main(int argc, char *argv[]) {
  int i, A[500], B[500];
  
  #pragma gfn start
  
  for (i=0;i<500;i++) {
  	B[i] = i;
  }
  
  #pragma gfn data input(B[500]) output(A[500])
  {
  
  #pragma gfn parallel_for input(B[500]) output(A[500])
  for (i=0;i<500;i++) {
    A[i] = a(B[i],8);
  }
  
  } /* end acc data */
  
  
  #pragma gfn finish
  
  // Test result
  for (i=0;i<500;i++) {
  	if (A[i] != a(B[i],8)) {
  	  printf("Fail at i = %d , Expected = %d but A[i] = %d\n", i, a(B[i],8), A[i]);
      assert(0);
  	}
  }
  printf("TEST PASS!\n");
  
  return 0;
}
