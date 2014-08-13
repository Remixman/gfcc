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
    int *vec1, *vec2, *vec3;
    int it, iteration = 100, i, n = 51200;
    int passed = 1;
    long long time0, time1;
    
    vec1 = (int*) malloc(sizeof(int) * n);
    vec2 = (int*) malloc(sizeof(int) * n);
    vec3 = (int*) malloc(sizeof(int) * n);
    
    // Initialize
    for (i = 0; i < n; ++i) {
        vec1[i] = 1;
        vec2[i] = 2;
    }
    
    time0 = get_time();
    for (it = 0; it < iteration; ++it) {
        #pragma gfn parallel_for copyin(vec1[0:n{partition}],vec2[0:n{partition}]) \
            copyout(vec3[0:n{partition}])
        for (i = 0; i < n; ++i) {
            vec3[i] = vec1[i] + vec2[i];
        }
    }
    time1 = get_time();
    
    for (i = 0; i < n; ++i) {
        if (vec3[i] != 3) {
            printf("At %d : get %d expected %d\n", i, vec3[i], 3);
            passed = 0;
            break;
        }
    }
    
    free(vec1);
    free(vec2);
    free(vec3);

    if (passed) printf("PASSED\n");
    else printf("FAILED\n");
    printf("Average time = %f sec.\n", ((float)(time1-time0)/1000000)/iteration);
	
    return 0;
}

