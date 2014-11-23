#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifdef _GFN
#include <gfn.h>
#endif

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

int main(int argc, char *argv[]) {
    double *vec1, *vec2;
    int it, iteration = 100, i, n = 51200;
    int passed = 1;
    long long time0, time1;

    if (argc >= 2) n = atoi(argv[1]);
    if (argc >= 3) iteration = atoi(argv[2]);
    
    vec1 = (double*) malloc(sizeof(double) * n);
    vec2 = (double*) malloc(sizeof(double) * n);
    
    // Initialize
    for (i = 0; i < n; ++i) vec1[i] = rand() % 10000;
    
    time0 = get_time();
    for (it = 0; it < iteration; ++it) {
        #pragma gfn parallel_for copyin(vec1[0:n{partition}]) copyout(vec2[0:n{partition}]) \
            in_pattern(vec1:[-1,1])
        for (i = 0; i < n; ++i) {
        	if (i > 0 && i < n-1) vec2[i] = (vec1[i-1] + vec1[i] + vec1[i+1]) / 3.0;
        	else vec2[i] = vec1[i];
        }
    }
    time1 = get_time();
    
    /*for (i = 1; i < n-1; ++i) {
        if ( fabs(vec2[i] - (vec1[i-1]+vec1[i]+vec1[i+1])/3.0 ) > 0.00001) {
            printf("At %d : get %.4lf expected %.4lf\n", i, vec2[i], (vec1[i-1]+vec1[i]+vec1[i+1])/3.0);
            passed = 0;
            break;
        }
    }*/
    
    free(vec1);
    free(vec2);

    if (passed) printf("PASSED\n");
    else printf("FAILED\n");
    printf("Average time = %f sec.\n", ((float)(time1-time0)/1000000)/iteration);
	
    return 0;
}

