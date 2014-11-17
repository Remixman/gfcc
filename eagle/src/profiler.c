#include <gsl/gsl_multifit.h>
#include <math.h>
#include "profiler.h"

#define STACK_SIZE 8
#define DEGREE 3

/* chuck stack abstraction */
int _chunk_stack_idx;
int _chuck_stack[STACK_SIZE];
void push_chuck_stack(int chuck) { _chuck_stack[_chunk_stack_idx++] = chuck; }
int top_chuck_stack() { return _chuck_stack[_chunk_stack_idx-1]; }
int pop_chuck_stack() { _chunk_stack_idx--; }
int empty_check_stack() { return (_chunk_stack_idx == 0); }

/* application profiler */

#define SCATTER_TIME     0
#define UPLOAD_TIME      1
#define EXEC_TIME        2
#define DOWNLOAD_TIME    3 
#define GATHER_TIME      4
#define TIME_SIZE        5
int _time_idx[TIME_SIZE];
int _time_size[TIME_SIZE][STACK_SIZE];
double _time_stack[TIME_SIZE][STACK_SIZE];
void push_time_stack(int time_type, int esize, int etime) {
	int idx = _time_idx[time_type];
	_time_size[time_type][idx] = esize;
	_time_stack[time_type][idx] = etime;
	_time_idx[time_type]++;
}
void push_scatter_time(int esize, double etime) { push_time_stack(SCATTER_TIME, esize, etime); }
void push_upload_time(int esize, double etime) { push_time_stack(UPLOAD_TIME, esize, etime); }
void push_exec_time(int esize, double etime) { push_time_stack(EXEC_TIME, esize, etime); }
void push_download_time(int esize, double etime) { push_time_stack(DOWNLOAD_TIME, esize, etime); }
void push_gather_time(int esize, double etime) { push_time_stack(GATHER_TIME, esize, etime); }

/* platform profile */
//double 

/* estimation function */
double _coeff[DEGREE];
int polynomialfit(int obs, int degree, double *dx, double *dy, double *store) {
	gsl_multifit_linear_workspace *ws;
	gsl_matrix *cov, *X;
	gsl_vector *y, *c;
	double chisq;

	int i, j;

	X = gsl_matrix_alloc(obs, degree);
	y = gsl_vector_alloc(obs);
	c = gsl_vector_alloc(degree);
	cov = gsl_matrix_alloc(degree, degree);

	for(i=0; i < obs; i++) {
		gsl_matrix_set(X, i, 0, 1.0);
		for(j=0; j < degree; j++) {
			gsl_matrix_set(X, i, j, pow(dx[i], j));
		}
		gsl_vector_set(y, i, dy[i]);
	}

	ws = gsl_multifit_linear_alloc(obs, degree);
	gsl_multifit_linear(X, y, c, cov, &chisq, ws);

	/* store result ... */
	for(i=0; i < degree; i++)
	{
		store[i] = gsl_vector_get(c, i);
	}

	gsl_multifit_linear_free(ws);
	gsl_matrix_free(X);
	gsl_matrix_free(cov);
	gsl_vector_free(y);
	gsl_vector_free(c);

	return 1; /* we do not "analyse" the result (cov matrix mainly) to know if the fit is "good" */
}
double time_predict(double x) {
  return (_coeff[2]*x*x) + (_coeff[1]*x) + (_coeff[0]);
}



void init_profiler(int local_loop_size) {
	int i;
	
	_chunk_stack_idx = 0;
	for (i = 0; i < TIME_SIZE; i++) _time_idx[i] = 0;
}


