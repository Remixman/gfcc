#include <stdio.h>
#include <assert.h>
#include <gfn.h>

#define PROBLEM_SIZE 10
#define	IMAX	PROBLEM_SIZE
#define	JMAX	PROBLEM_SIZE
#define	KMAX	PROBLEM_SIZE

/* COMMON block: global */
static int grid_points[3];	/* grid_ponts(1:3) */

static double us[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
/*static double vs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static double ws[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static double qs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static double rho_i[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static double square[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static double forcing[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][5+1];
static double u[(IMAX+1)/2*2+1][(JMAX+1)/2*2+1][(KMAX+1)/2*2+1][5];
static double rhs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][5];
static double lhs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][3][5][5];*/

int main() {
	
	int i, j, k, m;
  	double rho_inv, uijk, up1, um1, vijk, vp1, vm1, wijk, wp1, wm1;
  	int gp0, gp1, gp2;
  	int isize, jsize, ksize;
	
	#pragma gfn start
	
	gp0 = grid_points[0], gp1 = grid_points[1], gp2 = grid_points[2];
	isize = IMAX/2*2+1;
	jsize = JMAX/2*2+1; 
	ksize = KMAX/2*2+1;
	// TODO: remove input(gp0,gp1,gp2)
	#pragma gfn parallel_for output(us[isize][jsize][ksize]) input(gp0,gp1,gp2)
 	for (i = 0; i < gp0; i++) {
 		for (j = 0; j < gp1; j++) {
			for (k = 0; k < gp2; k++) {
				us[i][j][k] = i + j + k;
			}
		}
	}

	/*#pragma gfn parallel_for input(grid_points,)
  for (i = 0; i < grid_points[0]; i++) {
    for (j = 0; j < grid_points[1]; j++) {
      for (k = 0; k < grid_points[2]; k++) {
	rho_inv = 1.0/u[i][j][k][0];
	rho_i[i][j][k] = rho_inv;
	us[i][j][k] = u[i][j][k][1] * rho_inv;
	vs[i][j][k] = u[i][j][k][2] * rho_inv;
	ws[i][j][k] = u[i][j][k][3] * rho_inv;
	square[i][j][k] = 0.5 * (u[i][j][k][1]*u[i][j][k][1] + 
				 u[i][j][k][2]*u[i][j][k][2] +
				 u[i][j][k][3]*u[i][j][k][3] ) * rho_inv;
	qs[i][j][k] = square[i][j][k] * rho_inv;
      }
    }
  }*/

	
	#pragma gfn finish

	
	// Test result
	for (i = 0; i < gp0; i++) {
 		for (j = 0; j < gp1; j++) {
			for (k = 0; k < gp2; k++) {
				double diff = us[i][j][k] - (i + j + k);
				if (diff > 0.0001 || -0.0001 < diff) {
					printf("Failed us[%d][%d][%d] = %.5lf but expected is %.5lf\n", i, j, k, us[i][j][k], i+j+k);
					assert(0);
				}
			}
		}
	}
	printf("TEST PASS!\n");

	return 0;
}

