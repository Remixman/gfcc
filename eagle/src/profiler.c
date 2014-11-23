#include <gsl/gsl_multifit.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "profiler.h"

#define STACK_SIZE 6
#define DEGREE 2

#define MAX(A,B) ((A)<(B)?(B):(A))

int _local_loop_size;

/* 
 * 0 | 1 2 3 4 | 5 6 7 8 9 10 11 | 12 13 14 15 ... 
 * Array : 0 4 11
 */
int _split_time_function_idx[20];  // if split i and i+1 insert i
int _split_idx_idx = 0;

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
void push_exec_size(int esize) { push_size_stack(EXEC_TIME, esize); }
void push_exec_time(double etime) { push_time_stack(EXEC_TIME, etime); }
int full_exec_time_stack() { return full_time_stack(EXEC_TIME); }


/* platform profile */
#define MAX_MPI_LOG 300
int _scatter_size_list[MAX_MPI_LOG];
double _scatter_time_list[MAX_MPI_LOG];
int _scatter_list_idx = 0;
void push_scatter_time(int size, double stime) {
    _scatter_size_list[_scatter_list_idx] = size;
    _scatter_time_list[_scatter_list_idx] = stime;
    _scatter_list_idx++;
}
int _gather_size_list[MAX_MPI_LOG];
double _gather_time_list[MAX_MPI_LOG];
int _gather_list_idx = 0;
void push_gather_time(int size, double stime) {
    _gather_size_list[_gather_list_idx] = size;
    _gather_time_list[_gather_list_idx] = stime;
    _gather_list_idx++;
}
double _min_estimate_time = 50000;
double _max_estimate_time = 0;
double _estimate_time_range;
double _estimate_time_list[MAX_MPI_LOG];


/* estimation function */
double _exec_coeff_1[DEGREE];
double _exec_coeff_2[DEGREE];
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
double time_predict(double x, double coeff[]) {
	assert(_created_exec_time_function > 0);
	return (coeff[1]*x) + coeff[0];
}
int _exec_time_not_create = 1;
int _opt_size = 0;
int create_exec_time_function(int rank) {
    
    if (rank != 0 && (_time_idx[EXEC_TIME]>=STACK_SIZE) && _exec_time_not_create == 1) {
        _exec_time_not_create = 0;
        _created_exec_time_function = 1;
        
        return 1;
    }
    
    if ((_time_idx[EXEC_TIME]>=STACK_SIZE) && _exec_time_not_create == 1) {
        
        int s, maxi, factor, i = 0;
        double left_bound_time = 0;
        double right_bound_time = 0;
        double min_time = 1000000;
        int min_time_idx = 0;
        double pred_exec_time;
        
        // estimate exec time function
        int max_diff_time_idx = 0;
        double diff, max_diff = 0;
        for (i = 1; i < STACK_SIZE; i++) {
            factor = (_local_loop_size / _time_size[EXEC_TIME][i]);
            diff = fabs(_time_stack[EXEC_TIME][i] - _time_stack[EXEC_TIME][i-1]);
            if (diff > max_diff) {
                max_diff_time_idx = i;
                max_diff = diff;
            }
        }
        
        // estimate function of second part
        {
            // estimate function of first part
            int n = STACK_SIZE - max_diff_time_idx;
            double x[n], y[n];
            
            polynomialfit(n, DEGREE, x, y, _exec_coeff_1);
        }
        
        // estimate function of second part
        {
            int n = max_diff_time_idx;
            double x[n], y[n];
            
            polynomialfit(n, DEGREE, x, y, _exec_coeff_2);
            time_predict(, _exec_coeff_2);
        
            //printf("COEFF 1 : %.5lf\n", _exec_coeff_2[1]);
            //printf("COEFF 0 : %.5lf\n", _exec_coeff_2[0]);
        }
        
        for (i = 0; i < _gather_list_idx; i++) {
            int size = _scatter_size_list[i];
            if (size <= _time_stack[EXEC_TIME][max_diff_time_idx])
                pred_exec_time = time_predict(size, _exec_coeff_1);
            else
                pred_exec_time = time_predict(size, _exec_coeff_2);
            _estimate_time_list[i] = MAX(_estimate_time_list[i], pred_exec_time);
            
            printf("%d\t%.16lf\n", size, _estimate_time_list[i]);
            
            if (_estimate_time_list[i] < min_time) {
                min_time = _estimate_time_list[i];
                min_time_idx = i;
            }
        }
        
        printf("Separate at %d value = %d\n", max_diff_time_idx, _time_size[EXEC_TIME][max_diff_time_idx]);
        // TODO: merge exec time to estimate function here
#if 0
            // merge with exec time
            _estimate_time_list[i] = MAX(_estimate_time_list[i])
#endif

        
        // find minimum time from each section
        /*for (s = 0; s < _split_idx_idx; s++) {
            maxi = _split_time_function_idx[i];

            if (_estimate_time_list[i] < _estimate_time_list[maxi])
                if (_estimate_time_list[i] < min_time_idx)
                    min_time_idx = _estimate_time_list[i];
            else
                if (_estimate_time_list[maxi] < min_time_idx)
                    min_time_idx = _estimate_time_list[maxi];
            
            i = maxi + 1;
        }*/
        
        _exec_time_not_create = 0;
        _created_exec_time_function = 1;
        
        printf("OPTIMAL SIZE IS : %d\n", _scatter_size_list[min_time_idx]);
        return _scatter_size_list[min_time_idx];
    }
	
	return 0;
}
#if 0
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
		
		//printf("MIN : %d - MAX : %d - TIME : %.12lf\n", (int)min_bound, (int)max_bound, time_predict(mid_size));
		
		if (fabs(ay-by) < 0.000000001) break;
		else if (ay < by) { max_bound = mid_size; }
		else /*if (ay > by)*/ { min_bound = max_bound; }
	}
    
	return mid_size;
}
#endif
void init_profiler(int local_loop_size, int rank) {
	int i;
	
	_local_loop_size = local_loop_size;
	
	_chunk_stack_idx = 0;
	for (i = 0; i < TIME_SIZE; i++) {
		_size_idx[i] = 0;
		_time_idx[i] = 0;
	}
	
	push_chuck_stack(14016);
	push_chuck_stack(28032);
	push_chuck_stack(56064);
	push_chuck_stack(112128);
	push_chuck_stack(224256);
    push_chuck_stack(448512);
    
    if (rank != 0) return;
    
    /* Create estimate time list */
    for (i = 0; i < _gather_list_idx; i++) {
        int size = _gather_size_list[i];
        int factor = ceil((double)_local_loop_size / (size));
        double est_time = MAX(_scatter_time_list[i] * factor, _gather_time_list[i] * factor);
        
        if (est_time > _max_estimate_time) _max_estimate_time = est_time;
        if (est_time < _min_estimate_time) _min_estimate_time = est_time;
        _estimate_time_list[i] = est_time;
    }
    _estimate_time_range = _max_estimate_time - _min_estimate_time;
    
    /* find split function point */
    for (i = 1; i < _gather_list_idx; i++) {
        double alpha = 0.3;
        double threshold = alpha * _estimate_time_range;
        if (fabs(_estimate_time_list[i] - _estimate_time_list[i-1]) > threshold) {
            printf("Split %.10lf and %.10lf\n", _scatter_time_list[i-1], _scatter_time_list[i]);
            _split_time_function_idx[_split_idx_idx++] = i-1;
        }
    }
}

void trace_exec_time() {
	
	/*FILE *f = fopen("exec_time.txt", "w");
	
	int i;
	for (i = 0; i < STACK_SIZE; i++) {
		int chuck = _time_size[EXEC_TIME][i];
		int seq_num = (_local_loop_size / chuck);
		fprintf(f, "%d\t%.7lf\n", chuck, seq_num * _time_stack[EXEC_TIME][i]);
	}
	
	fclose(f);*/
}
