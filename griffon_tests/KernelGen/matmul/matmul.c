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
	int name = atoi(arg); \
	if (name < 0) \
	{ \
		printf("Value for " #name " is invalid: %d\n", name); \
		exit(1); \
	}

#define double_rand() (((double)(rand() / (double)RAND_MAX) - 0.5) * 2)

void matmul_(int nx, int ny, int nz, double** A, double** B, double** C)
{
    int i, j, k;
    double Cij;
    
    #pragma gfn parallel_for copyin(A[0:nx{partition}][0:ny]) \
	    copyin(B[0:ny][0:nz]) copyout(C[0:nx{partition}][0:nz])
    for (i = 0; i < nx; i++)
    {
        #pragma gfn loop
        for (j = 0; j < nz; j++)
        {
            Cij = 0.0f;
            for (k = 0; k < ny; k++)
            {
                Cij += A[i][k] * B[k][j];
            }
            C[i][j] = Cij;
        }  
    }
}

int main(int argc, char* argv[])
{
    int i, j, it;
    double meanA = 0.0f;
    double meanB = 0.0f;
    double meanC = 0.0f;
    long long time0, time1;

	if (argc != 5)
	{
		printf("Usage: %s <nx> <ny> <ns> <nt>\n", argv[0]);
		exit(1);
	}

	parse_arg(nx, argv[1]);
	parse_arg(ny, argv[2]);
	parse_arg(ns, argv[3]);
	parse_arg(nt, argv[4]);

	double** A = (double**)malloc(nx * sizeof(double*));
	A[0] = (double*)malloc(nx * ny * sizeof(double));
	for (i = 1; i < nx; i++) A[i] = A[i-1] + ny;
	double** B = (double**)malloc(ny * sizeof(double*));
	B[0] = (double*)malloc(ny * ns * sizeof(double));
	for (i = 1; i < ny; i++) B[i] = B[i-1] + ns;
	double** C = (double**)malloc(nx * sizeof(double*));
	C[0] = (double*)malloc(nx * ns * sizeof(double));
	for (i = 1; i < nx; i++) C[i] = C[i-1] + ns;

	if (!A || !B || !C)
	{
		printf("Error allocating memory for arrays: %p, %p, %p\n", A, B, C);
		exit(1);
	}

	for (i = 0; i < nx; i++)
	{
	    for (j = 0; j < ny; j++)
	    {
		    A[i][j] = double_rand();
		    meanA += A[i][j];
		}
	}
	for (i = 0; i < ny; i++)
	{
	    for (j = 0; j < ns; j++)
	    {
		    B[i][j] = double_rand();
		    meanB += B[i][j];
		}
	}
	printf("initial mean = %f\n", (meanA / (nx * ny) + meanB / (ny * ns)));

	// warm up
	matmul_(nx, ny, ns, A, B, C);
	
	time0 = get_time();
	for (it = 0; it < nt; it++)
		matmul_(nx, ny, ns, A, B, C);
    time1 = get_time();

	// For the final mean - account only the norm of the top
	// most level (tracked by swapping idxs array of indexes).
	for (i = 0; i < ny; i++)
	    for (j = 0; j < ns; j++)
		    meanC += C[i][j];
	printf("final mean = %f\n", meanC / (ny * ns));
	
	printf("total time = %f sec.\n", (float)(time1-time0)/1000000);
	printf("average time = %f sec.\n", ((float)(time1-time0)/1000000)/nt);

    free(A[0]);
	free(A);
	free(B[0]);
	free(B);
	free(C[0]);
	free(C);

	return 0;
}

