#include <stdio.h>
#include <math.h>
#include <time.h>
#include <gfn.h>

double integrate(int n) {
	double sum, h, x;
	int i;
	h = 1.0 / (double) n;
	#pragma gfn parallel_for reduction(+:sum) input(h,n)
	for (i = 1; i <= n; i++) {
		x = h*((double)i - 0.5);
		sum += 4.0 / (1.0 + x*x);
	}
	return sum * h;
}

int main(int argc, char *argv[]) {
	int n, elapse;
	double PI25DT = 3.14159265358;
	double pi;
	
	#pragma gfn start
  
	while (1) {
		printf("Enter # intervals: ");
		scanf("%d",&n);
		if (n == 0)
			break;
		pi = integrate(n);

		printf("pi=%.16f, error=%.16f\n", pi, fabs(pi - PI25DT));
	}
	
	#pragma gfn finish
	
	return 0;
}

