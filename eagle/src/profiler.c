#include <gsl/gsl_multifit.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "profiler.h"

#define STACK_SIZE 8
#define DEGREE 3

int _local_loop_size;

/* chuck stack abstraction */
int _chunk_stack_idx;
int _chuck_stack[STACK_SIZE];
void push_chuck_stack(int chuck) { _chuck_stack[_chunk_stack_idx++] = chuck; }
int top_chuck_stack() { return _chuck_stack[_chunk_stack_idx-1]; }
void pop_chuck_stack() { _chunk_stack_idx--; }
int empty_check_stack() { return (_chunk_stack_idx == 0); }

/* application profiler */

#define SCATTER_TIME     0
#define UPLOAD_TIME      1
#define EXEC_TIME        2
#define DOWNLOAD_TIME    3 
#define GATHER_TIME      4
#define TIME_SIZE        5
int _size_idx[TIME_SIZE];
int _time_idx[TIME_SIZE];
int _time_size[TIME_SIZE][STACK_SIZE];
double _time_stack[TIME_SIZE][STACK_SIZE];
void push_size_stack(int time_type, int esize) {
	int idx = _size_idx[time_type];
	_time_size[time_type][idx] = esize;
	_size_idx[time_type]++;
}
void push_time_stack(int time_type, double etime) {
	int idx = _time_idx[time_type];
	_time_stack[time_type][idx] = etime;
	_time_idx[time_type]++;
}
int full_time_stack(int time_type) {
	return (_time_idx[time_type] == STACK_SIZE);
}
void push_scatter_size(int esize) { push_size_stack(SCATTER_TIME, esize); }
void push_upload_size(int esize) { push_size_stack(UPLOAD_TIME, esize); }
void push_exec_size(int esize) { push_size_stack(EXEC_TIME, esize); }
void push_download_size(int esize) { push_size_stack(DOWNLOAD_TIME, esize); }
void push_gather_size(int esize) { push_size_stack(GATHER_TIME, esize); }
void push_scatter_time(double etime) { push_time_stack(SCATTER_TIME, etime); }
void push_upload_time(double etime) { push_time_stack(UPLOAD_TIME, etime); }
void push_exec_time(double etime) { push_time_stack(EXEC_TIME, etime); }
void push_download_time(double etime) { push_time_stack(DOWNLOAD_TIME, etime); }
void push_gather_time(double etime) { push_time_stack(GATHER_TIME, etime); }
int full_scatter_time_stack() { return full_time_stack(SCATTER_TIME); }
int full_upload_time_stack() { return full_time_stack(UPLOAD_TIME); }
int full_exec_time_stack() { return full_time_stack(EXEC_TIME); }
int full_download_time_stack() { return full_time_stack(DOWNLOAD_TIME); }
int full_gather_time_stack() { return full_time_stack(GATHER_TIME); }


/* platform profile */
//double 

/* estimation function */
double _exec_coeff[DEGREE];
int _created_exec_time_function = 0;
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
	assert(_created_exec_time_function > 0);
	return (_exec_coeff[2]*x*x) + (_exec_coeff[1]*x) + (_exec_coeff[0]);
}
int _exec_time_not_create = 1;
int _opt_size = 0;
void create_exec_time_function() {
	int n = STACK_SIZE;
	double x[STACK_SIZE];
	double y[STACK_SIZE];
	int i;
	
	if ((_time_idx[EXEC_TIME]>=STACK_SIZE) && _exec_time_not_create == 1) {
		for (i = 0; i < STACK_SIZE; i++) {
			int seq_num = (_local_loop_size / _time_size[EXEC_TIME][i]);
			x[i] = _time_size[EXEC_TIME][i];
			y[i] = seq_num * _time_stack[EXEC_TIME][i];
		}
		
		polynomialfit(n, DEGREE, x, y, _exec_coeff);
		
		printf("COEFF 2 : %.5lf\n", _exec_coeff[2]);
		printf("COEFF 1 : %.5lf\n", _exec_coeff[1]);
		printf("COEFF 0 : %.5lf\n", _exec_coeff[0]);
		
		_exec_time_not_create = 0;
		_created_exec_time_function = 1;
        
		_opt_size = find_optimal_size();
		printf("OPTIMAL SIZE : %d\n", _opt_size);
	}
}
int find_optimal_size() {
	double min_bound = 14016;
	double max_bound = _local_loop_size / 10;
	double mid_size, ax, ay, bx, by;
	mid_size = (min_bound + max_bound) / 2.0;
	
	// bisection method
	while (1) {
		if (fabs(min_bound - max_bound) < 1) break;
		
		mid_size = (min_bound + max_bound) / 2.0;
		ax = (mid_size + min_bound) / 2.0;
		bx = (mid_size + max_bound) / 2.0;
		
		ay = time_predict(ax);
		by = time_predict(bx);
		
		printf("MIN : %d - MAX : %d - TIME : %.12lf\n", (int)min_bound, (int)max_bound, time_predict(mid_size));
		
		if (fabs(ay-by) < 0.000000001) break;
		else if (ay < by) { max_bound = mid_size; }
		else /*if (ay > by)*/ { min_bound = max_bound; }
	}
    
	return mid_size;
}

void init_profiler(int local_loop_size) {
	int i;
	
	_local_loop_size = local_loop_size;
	
	_chunk_stack_idx = 0;
	for (i = 0; i < TIME_SIZE; i++) {
		_size_idx[i] = 0;
		_time_idx[i] = 0;
	}
	
	push_chuck_stack(14016);
	push_chuck_stack(28032);
	push_chuck_stack(42048);
	push_chuck_stack(56064);
	push_chuck_stack(112128);
	push_chuck_stack(126144);
	push_chuck_stack(182208);
	push_chuck_stack(210240);
}

void trace_exec_time() {
	
	FILE *f = fopen("exec_time.txt", "w");
	
	int i;
	for (i = 0; i < STACK_SIZE; i++) {
		int chuck = _time_size[EXEC_TIME][i];
		int seq_num = (_local_loop_size / chuck);
		fprintf(f, "%d\t%.7lf\n", chuck, seq_num * _time_stack[EXEC_TIME][i]);
	}
	
	fclose(f);
}
