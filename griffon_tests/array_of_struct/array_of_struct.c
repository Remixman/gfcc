#include <stdio.h>
#include <assert.h>
#ifdef _GRIFFON
#include <gfn.h>
#endif

struct coor {
    int x;
    int y;
    int z;
};

int main(int argc, char *argv[]) {
    int i, n = 500;
    struct coor points[n];
  
    #pragma gfn start

    #pragma gfn parallel_for output(points[n])
    for (i = 0; i < n; ++i) {
        points[i].x = i;
        points[i].y = i;
        points[i].z = i;
    }  
  
    #pragma gfn parallel_for inout(points[n])
    for (i = 0; i < n; ++i) {
        points[i].x *= -1;
        points[i].y *= -1;
        points[i].z *= -1;
    }  
  
    #pragma gfn finish
  
    // Test result
    for (i = 0; i < n; ++i) {
        assert(points[i].x==-i);
        assert(points[i].y==-i);
        assert(points[i].z==-i);
    }  
    printf("TEST PASS!\n");
  
    return 0;
}

