#include <stdio.h>
#include <stdlib.h>
#ifdef _GRIFFON
#include <gfn.h>
#endif

int main(int argc, char *argv[]) {
  int i, *A, n = 5000;
  int expected_result = (n*(n+1)) / 2;
  int error = 0;
  int s1 = 0;
  float s2 = 0;
  
  #pragma gfn start
  
  #pragma gfn parallel_for reduction(+:s1,s2)
  for (i=1;i<=n;i++) {
    s1 += i;
    s2 += (float)i;
  }
  
  if (expected_result != s1) {
  	printf("s1 result error!! (Expected is [%d] but s1 is [%d])\n", expected_result, s1);
  	error = 1;
  }
  if (expected_result != s2) {
  	printf("s2 result error!! (Expected is [%d] but s2 is [%d])\n", expected_result, (int)s2);
  	error = 1;
  }
  
  if (!error) {
  	printf("TEST PASS!\n");
  }
  
  #pragma gfn finish
  
  return 0;
}
