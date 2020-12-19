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

#define _A(array, ix, iy, is) (array[(ix) + nx * (iy) + nx * ny * (is)])

#if defined(_MIC)
__attribute__((target(mic)))
#endif
#if defined(__CUDACC__)
extern "C" __global__
#endif
void tricubic(int nx, int ny, int ns,
#if defined(__CUDACC__)
	int i_stride, int j_stride, int k_stride,
#endif
	double* u0, double* u1, double* a, double* b, double* c)
{
#if defined(__CUDACC__)
	#define k_offset (blockIdx.z * blockDim.z + threadIdx.z)
	#define j_offset (blockIdx.y * blockDim.y + threadIdx.y)
	#define i_offset (blockIdx.x * blockDim.x + threadIdx.x)
	#define k_increment k_stride
	#define j_increment j_stride
	#define i_increment i_stride
#else
	#define k_offset 0
	#define j_offset 0
	#define i_offset 0
	#define k_increment 1
	#define j_increment 1
	#define i_increment 1
#endif
#if defined(_OPENACC)
	size_t szarray = (size_t)nx * ny * ns;
	#pragma acc kernels loop independent gang(65535), present(u0[0:szarray], u1[0:szarray], a[0:szarray], b[0:szarray], c[0:szarray])
#endif
#if defined(_OPENMP) || defined(_MIC)
	#pragma omp parallel for
#endif
	for (int k = 1 + k_offset; k < ns - 2; k += k_increment)
	{
#if defined(_OPENACC)
		#pragma acc loop independent
#endif
		for (int j = 1 + j_offset; j < ny - 2; j += j_increment)
		{
#if defined(_OPENACC)
			#pragma acc loop independent vector(512)
#endif
			for (int i = 1 + i_offset; i < nx - 2; i += i_increment)
			{
				double w1_a =  1.0/6.0                      * _A(a, i,j,k) * (_A(a, i,j,k)+1.0) * (_A(a, i,j,k)+2.0);
				double w2_a = -0.5     * (_A(a, i,j,k)-1.0)                * (_A(a, i,j,k)+1.0) * (_A(a, i,j,k)+2.0);
				double w3_a =  0.5     * (_A(a, i,j,k)-1.0) * _A(a, i,j,k)                      * (_A(a, i,j,k)+2.0);
				double w4_a = -1.0/6.0 * (_A(a, i,j,k)-1.0) * _A(a, i,j,k) * (_A(a, i,j,k)+1.0);

				double w1_b =  1.0/6.0                      * _A(b, i,j,k) * (_A(b, i,j,k)+1.0) * (_A(b, i,j,k)+2.0);
				double w2_b = -0.5     * (_A(b, i,j,k)-1.0)                * (_A(b, i,j,k)+1.0) * (_A(b, i,j,k)+2.0);
				double w3_b =  0.5     * (_A(b, i,j,k)-1.0) * _A(b, i,j,k)                      * (_A(b, i,j,k)+2.0);
				double w4_b = -1.0/6.0 * (_A(b, i,j,k)-1.0) * _A(b, i,j,k) * (_A(b, i,j,k)+1.0);

				double w1_c =  1.0/6.0                      * _A(c, i,j,k) * (_A(c, i,j,k)+1.0) * (_A(c, i,j,k)+2.0);
				double w2_c = -0.5     * (_A(c, i,j,k)-1.0)                * (_A(c, i,j,k)+1.0) * (_A(c, i,j,k)+2.0);
				double w3_c =  0.5     * (_A(c, i,j,k)-1.0) * _A(c, i,j,k)                      * (_A(c, i,j,k)+2.0);
				double w4_c = -1.0/6.0 * (_A(c, i,j,k)-1.0) * _A(c, i,j,k) * (_A(c, i,j,k)+1.0);

				_A(u1, i,j,k) =
				
					w1_a * w1_b * w1_c * _A(u0, i-1, j-1, k-1) +	// w(1,1,1) = w1(a) w1(b) w1(c)
					w2_a * w1_b * w1_c * _A(u0, i  , j-1, k-1) +
					w3_a * w1_b * w1_c * _A(u0, i+1, j-1, k-1) +
					w4_a * w1_b * w1_c * _A(u0, i+2, j-1, k-1) +
			
					w1_a * w2_b * w1_c * _A(u0, i-1, j  , k-1) +
					w2_a * w2_b * w1_c * _A(u0, i  , j  , k-1) +
					w3_a * w2_b * w1_c * _A(u0, i+1, j  , k-1) +
					w4_a * w2_b * w1_c * _A(u0, i+2, j  , k-1) +

					w1_a * w3_b * w1_c * _A(u0, i-1, j+1, k-1) +
					w2_a * w3_b * w1_c * _A(u0, i  , j+1, k-1) +
					w3_a * w3_b * w1_c * _A(u0, i+1, j+1, k-1) +
					w4_a * w3_b * w1_c * _A(u0, i+2, j+1, k-1) +

					w1_a * w4_b * w1_c * _A(u0, i-1, j+2, k-1) +
					w2_a * w4_b * w1_c * _A(u0, i  , j+2, k-1) +
					w3_a * w4_b * w1_c * _A(u0, i+1, j+2, k-1) +
					w4_a * w4_b * w1_c * _A(u0, i+2, j+2, k-1) +


					w1_a * w1_b * w2_c * _A(u0, i-1, j-1, k  ) +
					w2_a * w1_b * w2_c * _A(u0, i  , j-1, k  ) +
					w3_a * w1_b * w2_c * _A(u0, i+1, j-1, k  ) +
					w4_a * w1_b * w2_c * _A(u0, i+2, j-1, k  ) +
			
					w1_a * w2_b * w2_c * _A(u0, i-1, j  , k  ) +
					w2_a * w2_b * w2_c * _A(u0, i  , j  , k  ) +
					w3_a * w2_b * w2_c * _A(u0, i+1, j  , k  ) +
					w4_a * w2_b * w2_c * _A(u0, i+2, j  , k  ) +

					w1_a * w3_b * w2_c * _A(u0, i-1, j+1, k  ) +
					w2_a * w3_b * w2_c * _A(u0, i  , j+1, k  ) +
					w3_a * w3_b * w2_c * _A(u0, i+1, j+1, k  ) +
					w4_a * w3_b * w2_c * _A(u0, i+2, j+1, k  ) +

					w1_a * w4_b * w2_c * _A(u0, i-1, j+2, k  ) +
					w2_a * w4_b * w2_c * _A(u0, i  , j+2, k  ) +
					w3_a * w4_b * w2_c * _A(u0, i+1, j+2, k  ) +
					w4_a * w4_b * w2_c * _A(u0, i+2, j+2, k  ) +


					w1_a * w1_b * w3_c * _A(u0, i-1, j-1, k+1) +
					w2_a * w1_b * w3_c * _A(u0, i  , j-1, k+1) +
					w3_a * w1_b * w3_c * _A(u0, i+1, j-1, k+1) +
					w4_a * w1_b * w3_c * _A(u0, i+2, j-1, k+1) +
			
					w1_a * w2_b * w3_c * _A(u0, i-1, j  , k+1) +
					w2_a * w2_b * w3_c * _A(u0, i  , j  , k+1) +
					w3_a * w2_b * w3_c * _A(u0, i+1, j  , k+1) +
					w4_a * w2_b * w3_c * _A(u0, i+2, j  , k+1) +

					w1_a * w3_b * w3_c * _A(u0, i-1, j+1, k+1) +
					w2_a * w3_b * w3_c * _A(u0, i  , j+1, k+1) +
					w3_a * w3_b * w3_c * _A(u0, i+1, j+1, k+1) +
					w4_a * w3_b * w3_c * _A(u0, i+2, j+1, k+1) +

					w1_a * w4_b * w3_c * _A(u0, i-1, j+2, k+1) +
					w2_a * w4_b * w3_c * _A(u0, i  , j+2, k+1) +
					w3_a * w4_b * w3_c * _A(u0, i+1, j+2, k+1) +
					w4_a * w4_b * w3_c * _A(u0, i+2, j+2, k+1) +


					w1_a * w1_b * w4_c * _A(u0, i-1, j-1, k+2) +
					w2_a * w1_b * w4_c * _A(u0, i  , j-1, k+2) +
					w3_a * w1_b * w4_c * _A(u0, i+1, j-1, k+2) +
					w4_a * w1_b * w4_c * _A(u0, i+2, j-1, k+2) +
			
					w1_a * w2_b * w4_c * _A(u0, i-1, j  , k+2) +
					w2_a * w2_b * w4_c * _A(u0, i  , j  , k+2) +
					w3_a * w2_b * w4_c * _A(u0, i+1, j  , k+2) +
					w4_a * w2_b * w4_c * _A(u0, i+2, j  , k+2) +

					w1_a * w3_b * w4_c * _A(u0, i-1, j+1, k+2) +
					w2_a * w3_b * w4_c * _A(u0, i  , j+1, k+2) +
					w3_a * w3_b * w4_c * _A(u0, i+1, j+1, k+2) +
					w4_a * w3_b * w4_c * _A(u0, i+2, j+1, k+2) +

					w1_a * w4_b * w4_c * _A(u0, i-1, j+2, k+2) +
					w2_a * w4_b * w4_c * _A(u0, i  , j+2, k+2) +
					w3_a * w4_b * w4_c * _A(u0, i+1, j+2, k+2) +
					w4_a * w4_b * w4_c * _A(u0, i+2, j+2, k+2);
			}
		}
	}
}

#define parse_arg(name, arg) \
	int name = atoi(arg); \
	if (name < 0) \
	{ \
		printf("Value for " #name " is invalid: %d\n", name); \
		exit(1); \
	}

#define double_rand() (((double)(rand() / (double)RAND_MAX) - 0.5) * 2)

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("Usage: %s <nx> <ny> <ns> <nt>\n", argv[0]);
		exit(1);
	}

	const char* no_timing = getenv("NO_TIMING");

#if defined(_OPENACC) || defined(__CUDACC__)
	char* regcount_fname = getenv("PROFILING_FNAME");
	if (regcount_fname)
	{
		char* regcount_lineno = getenv("PROFILING_LINENO");
		int lineno = -1;
		if (regcount_lineno)
			lineno = atoi(regcount_lineno);
		kernelgen_enable_regcount(regcount_fname, lineno);
	}
#endif

	parse_arg(nx, argv[1]);
	parse_arg(ny, argv[2]);
	parse_arg(ns, argv[3]);
	parse_arg(nt, argv[4]);

	size_t szarray = (size_t)nx * ny * ns;
	size_t szarrayb = szarray * sizeof(double);

	double* u0 = (double*)memalign(MEMALIGN, szarrayb);
	double* u1 = (double*)memalign(MEMALIGN, szarrayb);
	double* a = (double*)memalign(MEMALIGN, szarrayb);
	double* b = (double*)memalign(MEMALIGN, szarrayb);
	double* c = (double*)memalign(MEMALIGN, szarrayb);

	if (!u0 || !u1 || !a || !b || !c)
	{
		printf("Error allocating memory for arrays: %p, %p, %p, %p, %p\n", u0, u1, a, b, c);
		exit(1);
	}

	double mean = 0.0f;
	for (int i = 0; i < szarray; i++)
	{
		u0[i] = double_rand();
		u1[i] = double_rand();
		a[i] = double_rand();
		b[i] = double_rand();
		c[i] = double_rand();
		mean += u0[i] + u1[i] + a[i] + b[i] + c[i];
	}
	printf("initial mean = %f\n", mean / szarray / 5);

	#pragma acc update device(u0[0:szarray], u1[0:szarray], a[0:szarray], b[0:szarray], c[0:szarray])

	//
	// 4) Perform data processing iterations, keeping all data
	// on device.
	//
	int idxs[] = { 0, 1 };
	volatile struct timespec compute_s, compute_f;
	get_time(&compute_s);
#if defined(_MIC)
	#pragma offload target(mic) \
		nocopy(u0:length(szarray) alloc_if(0) free_if(0)), \
		nocopy(u1:length(szarray) alloc_if(0) free_if(0)), \
		nocopy(a:length(szarray) alloc_if(0) free_if(0)), \
		nocopy(b:length(szarray) alloc_if(0) free_if(0)), \
		nocopy(c:length(szarray) alloc_if(0) free_if(0))
#endif
#if defined(__CUDACC__)
	dim3 gridDim, blockDim, strideDim;
	kernelgen_cuda_configure_gird(nx, ny, ns, &gridDim, &blockDim, &strideDim);
#endif
	{
		double *u0p = u0, *u1p = u1;

		for (int it = 0; it < nt; it++)
		{
#if !defined(__CUDACC__)
			tricubic(nx, ny, ns, u0p, u1p, a, b, c);
#else
			tricubic<<<gridDim, blockDim>>>(nx, ny, ns,
				strideDim.x, strideDim.y, strideDim.z,
				u0p, u1p, a, b, c);
#endif
			double* u = u0p; u0p = u1p; u1p = u;
			int idx = idxs[0]; idxs[0] = idxs[1]; idxs[1] = idx;
		}
	}
	get_time(&compute_f);
	double compute_t = get_time_diff((struct timespec*)&compute_s, (struct timespec*)&compute_f);
	if (!no_timing) printf("compute time = %f sec\n", compute_t);

	double* u[] = { u0, u1 }; 
	u0 = u[idxs[0]]; u1 = u[idxs[1]];

	//
	// MIC or OPENACC or CUDA:
	//
	// 5) Transfer output data back from device to host.
	//
	#pragma acc update host (u1[0:szarray])


	// For the final mean - account only the norm of the top
	// most level (tracked by swapping idxs array of indexes).
	mean = 0.0f;
	for (int i = 0; i < szarray; i++)
		mean += u1[i];
	printf("final mean = %f\n", mean / szarray);

	free(u0);
	free(u1);
	free(a);
	free(b);
	free(c);

	return 0;
}

