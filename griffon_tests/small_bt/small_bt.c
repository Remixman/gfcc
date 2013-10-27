#include <stdio.h>
#include <assert.h>
#include <gfn.h>

#define PROBLEM_SIZE 3
#define	IMAX	PROBLEM_SIZE
#define	JMAX	PROBLEM_SIZE
#define	KMAX	PROBLEM_SIZE

/* COMMON block: global */
static int grid_points[3];	/* grid_ponts(1:3) */

static float us[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static float vs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static float ws[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static float qs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static float rho_i[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static float square[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1];
static float forcing[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][5+1];
static float u[(IMAX+1)/2*2+1][(JMAX+1)/2*2+1][(KMAX+1)/2*2+1][5];
static float rhs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][5];
static float lhs[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][3][5][5];

int main() {
	
	int i, j, k, m,n;
  	float rho_inv, uijk, up1, um1, vijk, vp1, vm1, wijk, wp1, wm1;
  	float dssp = 5.67;
  	int gp0, gp1, gp2;
  	int isize, jsize, ksize;
  	int uisize, ujsize, uksize;
	
	#pragma gfn start
	
  	gp0 = grid_points[0] = isize = IMAX/2*2+1;
	gp1 = grid_points[1] = jsize = JMAX/2*2+1; 
	gp2 = grid_points[2] = ksize = KMAX/2*2+1;

	uisize = (IMAX+1)/2*2+1;
	ujsize = (JMAX+1)/2*2+1;
	uksize = (KMAX+1)/2*2+1;

	// TODO: remove input(gp0,gp1,gp2)
	#pragma gfn parallel_for copyout(us[0:isize{partition}][0:jsize][0:ksize])
 	for (i = 0; i < gp0; i++) {
 		for (j = 0; j < gp1; j++) {
			for (k = 0; k < gp2; k++) {
				us[i][j][k] = i + j + k;
			}
		}
	}

	#pragma gfn parallel_for copyout(lhs[0:isize{partition}][0:jsize][0:ksize][0:3][0:5][0:5])
  for (i = 0; i < gp0; i++) {
    for (j = 0; j < gp1; j++) {
      for (k = 0; k < gp2; k++) {
	for (m = 0; m < 5; m++) {
	  for (n = 0; n < 5; n++) {
	    lhs[i][j][k][0][m][n] = 0.0;
	    lhs[i][j][k][1][m][n] = 0.0;
	    lhs[i][j][k][2][m][n] = 0.0;
	  }
	}
      }
    }
  }

	#pragma gfn parallel_for \
		copyin(u[0:uisize{partition}][0:ujsize][0:uksize][0:5]) \
		copyout(rho_i[0:isize{partition}][0:jsize][0:ksize]) \
		copyout(us[0:isize{partition}][0:jsize][0:ksize]) \
		copyout(vs[0:isize{partition}][0:jsize][0:ksize]) \
		copyout(ws[0:isize{partition}][0:jsize][0:ksize]) \
		copyout(qs[0:isize{partition}][0:jsize][0:ksize]) \
		copyout(square[0:isize{partition}][0:jsize][0:ksize])
	for (i = 0; i < gp0; i++) {
		for (j = 0; j < gp1; j++) {
			for (k = 0; k < gp2; k++) {
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
	}

	  i = 1;
#pragma gfn parallel_for \
	copy(rhs[0:isize{partition}][0:jsize][0:ksize][0:5]) \
	copyin(u[0:uisize{partition}][0:ujsize][0:uksize][0:5])
  for (j = 1; j < gp1-1; j++) {
    for (k = 1; k < gp2-1; k++) {
      for (m = 0; m < 5; m++) {
	rhs[i][j][k][m] = rhs[i][j][k][m]- dssp * 
	  ( 5.0*u[i][j][k][m] - 4.0*u[i+1][j][k][m] +
	    u[i+2][j][k][m]);
      }
    }
  }
	
	#pragma gfn finish

	
	// Test result
	/*for (i = 0; i < gp0; i++) {
 		for (j = 0; j < gp1; j++) {
			for (k = 0; k < gp2; k++) {
				float diff = us[i][j][k] - (i + j + k);
				if (diff > 0.0001 || -0.0001 > diff) {
					printf("Failed us[%d][%d][%d] = %.5lf but expected is %.5lf\n", i, j, k, us[i][j][k], i+j+k);
					assert(0);
				}
			}
		}
	}
	printf("TEST PASS!\n");*/

	return 0;
}

