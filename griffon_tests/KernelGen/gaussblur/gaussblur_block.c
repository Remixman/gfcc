//===----------------------------------------------------------------------===//
//
//     KernelGen -- A prototype of LLVM-based auto-parallelizing Fortran/C
//        compiler for NVIDIA GPUs, targeting numerical modeling code.
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _GFN
#include <gfn.h>
#endif

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

#define parse_arg(name, arg) \
	name = atoi(arg); \
	if (name < 0) \
	{ \
		printf("Value for " #name " is invalid: %d\n", name); \
		exit(1); \
	}

#define double_rand() (((double)(rand() / (double)RAND_MAX) - 0.5) * 2)

#define MIN(A,B) (((A)>(B))?(B):(A))

void gaussblur(int nx, int ny,
	const double s0, const double s1, const double s2,
	const double s4, const double s5, const double s8,
	double** w0, double** w1, int BLOCK_SIZE)
{
    int i, j;
    int i0, j0;
	double f = 1. / (s0 + 4 * (s1 + s2 + s4 + s8) + 8 * s5);

    for (i0 = 0; i0 < nx; i0 += BLOCK_SIZE)
    for (j0 = 0; j0 < nx; j0 += BLOCK_SIZE)
    {
        for (i = i0; i < MIN(nx, i0+BLOCK_SIZE-1); i++)
        {
            for (j = j0; j < MIN(nz, j0+BLOCK_SIZE-1); j++)
            {
                if (i >= 2 && i < nx-2 && j >= 2 && j < ny-2)
                    w1[i][j] = f * (
                        s0 * w0[i][j] +
                        s1 * (w0[i-1][ j ] + w0[i+1][ j ] + w0[ i ][j-1] + w0[ i ][j+1]) + 
                        s2 * (w0[i-1][j-1] + w0[i+1][j-1] + w0[i-1][j+1] + w0[i+1][j+1]) +
                        s4 * (w0[i-2][ j ] + w0[i+2][ j ] + w0[ i ][j-2] + w0[ i ][j+2]) +
                        s5 * (w0[i-2][j-1] + w0[i-1][j-2] + w0[i+1][j-2] + w0[i+2][j-1]  +
                            w0[i-2][j+1] + w0[i-1][j+2] + w0[i+1][j+2] + w0[i+2][j+1]) +
                        s8 * (w0[i-2][j-2] + w0[i+2][j-2] + w0[i-2][j+2] + w0[i+2][j+2]));
            }
        }
    }

	
    for (i0 = 0; i0 < nx; i0 += BLOCK_SIZE)
    for (j0 = 0; j0 < nx; j0 += BLOCK_SIZE)
    {
        for (i = i0; i < MIN(nx, i0+BLOCK_SIZE-1); i++)
        {
            for (j = j0; j < MIN(nz, j0+BLOCK_SIZE-1); j++)
            {
                w0[i][j] = w1[i][j];
            }
        }
    }
}

int main(int argc, char* argv[])
{
    int i, j, it;
    int nx, ny, nt;
    double **w0, **w1;
    double s0, s1, s2, s4, s5, s8; 
    double mean = 0.0f;
    long long time0, time1;

	if (argc != 5)
	{
		printf("Usage: %s <nx> <ny> <nt>\n", argv[0]);
		exit(1);
	}

	parse_arg(nx, argv[1]);
	parse_arg(ny, argv[2]);
	parse_arg(nt, argv[3]);
    parse_arg(BLOCK_SIZE, argv[4]);
	
	printf("nx = %d , ny = %d , nt = %d, b = %d\n", nx, ny, nt, BLOCK_SIZE);

	s0 = double_rand();
	s1 = double_rand();
	s2 = double_rand();
	s4 = double_rand();
	s5 = double_rand();
	s8 = double_rand();

	printf("s0 = %f, s1 = %f, s2 = %f\n", s0, s1, s2);
	printf("s4 = %f, s5 = %f, s8 = %f\n", s4, s5, s8);

	w0 = (double**)malloc(nx * sizeof(double*));
	w0[0] = (double*)malloc(nx * ny * sizeof(double));
	w1 = (double**)malloc(nx * sizeof(double*));
	w1[0] = (double*)malloc(nx * ny * sizeof(double));
	for (i = 1; i < nx; i++)
	{
	    w0[i] = w0[i-1] + ny;
	    w1[i] = w1[i-1] + ny;
	}

	if (!w0 || !w1)
	{
		printf("Error allocating memory for arrays: %p, %p\n", w0, w1);
		exit(1);
	}

	for (i = 0; i < nx; i++)
	{
	    for (j = 0; j < ny; j++)
	    {
		    w0[i][j] = double_rand();
		    w1[i][j] = double_rand();
		    mean += w0[i][j] + w1[i][j];
		}
	}
	printf("initial mean = %f\n", mean / (nx * ny) / 2);

    time0 = get_time();
    #pragma gfn data copy(w0[0:nx{partition}][0:ny]) \
        copyin(w1[0:nx{partition}][0:ny])
	{
		for (it = 0; it < nt; it++)
			gaussblur(nx, ny, s0, s1, s2, s4, s5, s8, w0, w1, BLOCK_SIZE);
	}
	time1 = get_time();

	// For the final mean - account only the norm of the top
	// most level (tracked by swapping idxs array of indexes).
	mean = 0.0f;
	for (i = 0; i < nx; i++)
	    for (j = 0; j < ny; j++)
	    	mean += w0[i][j];
	printf("final mean = %f\n", mean / (nx * ny));
	
	printf("total time = %f sec.\n", (float)(time1-time0)/1000000);
	printf("average time = %f sec.\n", ((float)(time1-time0)/1000000)/nt);

    free(w0[0]);
	free(w0);
	free(w1[0]);
	free(w1);

	return 0;
}

