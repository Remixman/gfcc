#include <math.h>
#include <stdio.h>
#include <stdlib.h>						/* getenv, */
#include <time.h>
#include <string.h>
#include "kernel_id_table.h"
#include "variable_id_table.h"
#include "gfn.h"


int _gfn_opt_level = 0;
int _gfn_rank;		/**/
int _gfn_num_proc;	/**/
cl_platform_id _gfn_platform_id;
cl_device_id _gfn_device_id;
cl_context _gfn_context;
cl_command_queue _gfn_cmd_queue;
cl_int _gfn_status;
cl_program _gfn_cl_program;

int _debug_stream_seq = 0;

/* buffer for reduction */
char *char_sum_buffer;
unsigned char *unsigned_char_sum_buffer;
short *short_sum_buffer;
unsigned short *unsigned_short_sum_buffer;
int *int_sum_buffer;
unsigned *unsigned_sum_buffer;
long *long_sum_buffer;
unsigned long *unsigned_long_sum_buffer;
float *float_sum_buffer;
double *double_sum_buffer;
long double *long_double_sum_buffer;
long long int *long_long_int_sum_buffer;

long long _gfn_last_kernel_time;
int is_overlap_node_dev_trans;
int stream_seq_factor = 1;

char current_kernel_name[50];

Ibcast_handle *send_scalar_handles[MAX_IBCAST_HANDLE_LIST];
int send_scalar_curr_idx;

static int _cluster_malloc_time;
static int _cluster_scatter_time;
static int _cluster_bcast_time;
static int _cluster_gather_time;
static int _gpu_malloc_time;
static int _gpu_kernel_time;
static int _gpu_transfer_h2d_time;
static int _gpu_transfer_d2h_time;
static int _mm_overhead_time;           /* Overhead time for memory management */	

static FILE * _trace_f;


#define IF_TIMING if
#define TRACE_LOG(...) if(_trace_f)fprintf(_trace_f, __VA_ARGS__);

#define TRUE 1
#define FALSE 0

#define PATTERN_NONE     0
#define PATTERN_RANGE    1
#define PATTERN_SPECIFIC 2

#define TYPE_CHAR             1
#define TYPE_UNSIGNED_CHAR    2
#define TYPE_BYTE             3
#define TYPE_SHORT            4
#define TYPE_UNSIGNED_SHORT   5
#define TYPE_INT              6
#define TYPE_UNSIGNED         7
#define TYPE_LONG             8
#define TYPE_UNSIGNED_LONG    9
#define TYPE_FLOAT            10
#define TYPE_DOUBLE           11
#define TYPE_LONG_DOUBLE      12
#define TYPE_LONG_LONG_INT    13

#define MAX(A,B) ((A)<(B)?(B):(A))
#define MIN(A,B) ((A)>(B)?(B):(A))

static long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

static int round_to(int x, int r) {
	int d = (x + r - 1) / r;
	return d * r;
}

// API for user
int gfn_get_num_process()
{
	return _gfn_num_proc;
}

int gfn_get_process_num()
{
	return _gfn_rank;
}

int _GfnInit(int *argc, char **argv[])
{
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;

	MPI_Init(argc, argv);
	MPI_Comm_size(MPI_COMM_WORLD, &_gfn_num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &_gfn_rank);
	MPI_Get_processor_name(processor_name, &name_len);
	
	printf("Rank %d is at %s\n", _gfn_rank, processor_name);

	/* Overlap transfer */
	is_overlap_node_dev_trans = FALSE;
	char opt_level = '0';
	char *opt_env = getenv("GFN_OPT");
	if (opt_env) opt_level = opt_env[0];

	MPI_Bcast(&opt_level, 1, MPI_CHAR, 0, MPI_COMM_WORLD);	
	if (_gfn_rank == 0) printf("GFN_OPT = %c\n", opt_level);

	if (opt_level != '0') {
		is_overlap_node_dev_trans = TRUE;
		_gfn_opt_level = opt_level - '0';
	}

	/* Send scalar list */
	send_scalar_curr_idx = 0;

	/* Chunk size */
	stream_seq_factor = 1;
	char *opt_stream_factor = getenv("GFN_OPT_STREAM_FACTOR");
	if (opt_stream_factor) sscanf(opt_stream_factor, "%d", &stream_seq_factor);
	MPI_Bcast(&stream_seq_factor, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (_gfn_rank == 0) printf("GFN_OPT_STREAM_FACTOR = %d\n", stream_seq_factor);


	/* Reduction buffer */
	double_sum_buffer = NULL;
	int_sum_buffer = NULL;
	float_sum_buffer = NULL;

	/* Trace timing */
	_cluster_malloc_time    = FALSE;
	_cluster_scatter_time   = FALSE;
	_cluster_bcast_time     = FALSE;
	_cluster_gather_time    = FALSE;
	_gpu_malloc_time        = FALSE;
	_gpu_kernel_time        = FALSE;
	_gpu_transfer_h2d_time  = FALSE;
	_gpu_transfer_d2h_time  = FALSE;
	_mm_overhead_time       = FALSE;

	char trace_level = '0';
	char *trace_env = getenv("GFN_TRACE");
	if (trace_env) trace_level = trace_env[0];

	MPI_Bcast(&trace_level, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	if (_gfn_rank == 0) printf("GFN_TRACE = %c\n", trace_level);

	if (trace_level != '0') {
	switch (trace_level) {
	case '5':
		_mm_overhead_time        = TRUE;
	case '4':
		_cluster_malloc_time     = TRUE;
		_gpu_malloc_time         = TRUE;
	case '3':
		_cluster_scatter_time    = TRUE;
		_cluster_bcast_time      = TRUE;
		_cluster_gather_time     = TRUE;
		_gpu_transfer_h2d_time   = TRUE;
		_gpu_transfer_d2h_time   = TRUE;
	case '2':
		_gpu_kernel_time         = TRUE;
	case '0':
		break;
	}

	// Open trace file
	_trace_f = fopen("trace_gfn.txt", "w");
	if (_trace_f == NULL) {
		fprintf(stderr, "Cannot open trace file [trace_gfn.txt]\n");
	}
	
	}

	_InitOpenCL();

	if (_gfn_rank == 0) _OpenWorkerMsgQueue();

	return 0;
}

int _GfnFinalize()
{
	long long clear_vtab_start_t, clear_vtab_end_t;

	IF_TIMING (_mm_overhead_time) clear_vtab_start_t = get_time();
	_free_mem_and_clear_var_table();
	IF_TIMING (_mm_overhead_time) clear_vtab_end_t = get_time();

	if (double_sum_buffer) free(double_sum_buffer);
	if (int_sum_buffer) free(int_sum_buffer);
	if (float_sum_buffer) free(float_sum_buffer);

	_clear_lock_table();

	_FinalOpenCL();

	if (_trace_f != NULL) {
		fclose(_trace_f);
	}
	
	MPI_Finalize();

	if (_gfn_rank == 0) _CloseWorkerMsgQueue();

	return 0;
}

int _GfnMallocReduceScalar(void * ptr, cl_mem *cl_ptr, int type_id, int group_num, int level1_cond, int level2_cond)
{
	long long create_buf_start_t, create_buf_end_t;

	// TODO: allocate for array memory
	if (level2_cond) {
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time();
		TRACE_LOG ("device-malloc start %lld\n", create_buf_start_t);
		(*cl_ptr) = clCreateBuffer(_gfn_context, CL_MEM_WRITE_ONLY, _CalcTypeSize(type_id) * group_num, 0, &_gfn_status);
		_GfnCheckCLStatus(_gfn_status, "CREATE REDUCE BUFFER");
		IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time();
		TRACE_LOG ("device-malloc end %lld\n", create_buf_end_t);
	}

	IF_TIMING (_gpu_malloc_time && level2_cond)
		printf("[%d] Allocate scalar %p on device : %.10f s.\n", _gfn_rank, ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);

	return 0;
}

int _GfnFreeReduceScalar(cl_mem cl_ptr, int level1_cond, int level2_cond)
{
	if (level2_cond) {
		_gfn_status = clReleaseMemObject(cl_ptr);
		_GfnCheckCLStatus(_gfn_status, "RELEASE REDUCE BUFFER");
	}
}

int _GfnEnqueueBroadcastScalar(void *ptr, int type_id)
{
	long long bcast_start_t, bcast_end_t;

	// TOD0: MPI pack and bcast at _GfnFinishBroadcastScalar

#define SWITCH_BCAST(mpi_type) \
do { \
	IF_TIMING (_cluster_bcast_time) bcast_start_t = get_time(); \
	TRACE_LOG ("node-transfer start %lld\n", bcast_start_t); \
	MPI_Bcast(ptr, 1, mpi_type, 0, MPI_COMM_WORLD); \
	IF_TIMING (_cluster_bcast_time) bcast_end_t = get_time(); \
	TRACE_LOG ("node-transfer end %lld\n", bcast_end_t); \
} while(0)

#define SWITCH_IBCAST(mpi_type) \
do { \
	Ibcast(ptr, 1, mpi_type, 0, MPI_COMM_WORLD, &(send_scalar_handles[send_scalar_curr_idx])); \
	send_scalar_curr_idx++; \
} while(0)

	if (_gfn_rank == 0) _RecvInputMsg(ptr, _CalcTypeSize(type_id));
    
	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_IBCAST(MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_IBCAST(MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_IBCAST(MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_IBCAST(MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_IBCAST(MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_IBCAST(MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_IBCAST(MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_IBCAST(MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_IBCAST(MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_IBCAST(MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_IBCAST(MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_IBCAST(MPI_LONG_LONG_INT); break;
	}

	IF_TIMING (_cluster_bcast_time)
		printf("[%d] Broadcast %p : %.10f s.\n", _gfn_rank, ptr, 
			(float)(bcast_end_t-bcast_start_t)/1000000);

	return 0;
}

int _GfnFinishBroadcastScalar()
{
	int i;
	for (i = 0; i < send_scalar_curr_idx; ++i)
		Ibcast_wait(&(send_scalar_handles[i]));

	send_scalar_curr_idx = 0;
	return 0;
}

int _GfnEnqueueReduceScalar(void *ptr, cl_mem cl_ptr, int type_id, MPI_Op op_id, int group_num, int level1_cond, int level2_cond)
{
	int i;

	// TODO: transfer reduce array from device and reduce again in host

	// TOD0: MPI pack and bcast at _GfnFinishReduceScalar

#define SWITCH_REDUCE(type,mpi_type,reduce_buf) \
do { \
	if (reduce_buf == NULL) reduce_buf = (type*) malloc(group_num * sizeof(type)); \
	type rv = 0; \
	if (level2_cond) { \
		_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * group_num, reduce_buf, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "READ BUFFER"); \
	} \
	for (i = 0; i < group_num; ++i) \
		rv += reduce_buf[i]; \
	MPI_Allreduce(&rv, ptr, 1, mpi_type, op_id, MPI_COMM_WORLD); \
} while(0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_REDUCE(char,MPI_CHAR,char_sum_buffer); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_REDUCE(unsigned char,MPI_UNSIGNED_CHAR,unsigned_char_sum_buffer); break;
	case TYPE_SHORT:          SWITCH_REDUCE(short,MPI_SHORT,short_sum_buffer); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_REDUCE(unsigned short,MPI_UNSIGNED_SHORT,unsigned_short_sum_buffer); break;
	case TYPE_INT:            SWITCH_REDUCE(int,MPI_INT,int_sum_buffer); break;
	case TYPE_UNSIGNED:       SWITCH_REDUCE(unsigned,MPI_UNSIGNED,unsigned_sum_buffer); break;
	case TYPE_LONG:           SWITCH_REDUCE(long,MPI_LONG,long_sum_buffer); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_REDUCE(unsigned long,MPI_UNSIGNED_LONG,unsigned_long_sum_buffer); break;
	case TYPE_FLOAT:          SWITCH_REDUCE(float,MPI_FLOAT,float_sum_buffer); break;
	case TYPE_DOUBLE:         SWITCH_REDUCE(double,MPI_DOUBLE,double_sum_buffer); break;
	case TYPE_LONG_DOUBLE:    SWITCH_REDUCE(long double,MPI_LONG_DOUBLE,long_double_sum_buffer); break;
	case TYPE_LONG_LONG_INT:  SWITCH_REDUCE(long long int,MPI_LONG_LONG_INT,long_long_int_sum_buffer); break;
	}

	if (_gfn_rank == 0) _SendOutputMsg(ptr, _CalcTypeSize(type_id));
	return 0;
}

int _GfnFinishReduceScalar()
{ 
	return 0;
}

int _GfnMalloc1D(void ** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, cl_mem_flags mem_type, int level1_cond, int level2_cond)
{
	long long malloc_start_t, malloc_end_t;
	long long create_buf_start_t, create_buf_end_t;
	long long insert_vtab_start_t, insert_vtab_end_t;

	cl_map_flags map_type;
	if (mem_type == CL_MEM_READ_ONLY) map_type = CL_MAP_READ;
	else if (mem_type == CL_MEM_WRITE_ONLY) map_type = CL_MAP_WRITE;
	else printf("ERROR : INVALID MEM TYPE FOR MAP TYPE\n");
	
	int found = 0;
	int retieve_dim_num = 0;
	cl_mem_flags dummy_mem_type = 0;
	_retieve_var_table(unique_id, cl_ptr, &dummy_mem_type, &retieve_dim_num, (void**)ptr, &found);
	if (found) {
		//printf("Don't Allocate 1D Again\n");
		return 0;
	}

	if (dim1_size <= 0) {
		fprintf(stdout, "ERROR %s:%d : Invalid size [%zu]\n", __FILE__, __LINE__, dim1_size);
	}
	
	if (_gfn_opt_level >= 3)
	{
		mem_type = mem_type | CL_MEM_ALLOC_HOST_PTR;
	}

#define SWITCH_MALLOC_1D(type,size1) \
do { \
	type * tmp_ptr; \
	IF_TIMING (_cluster_malloc_time) malloc_start_t = get_time(); \
	TRACE_LOG ("host-malloc start %lld\n", malloc_start_t); \
	tmp_ptr = (type *) malloc(sizeof(type) * size1); \
	IF_TIMING (_cluster_malloc_time) malloc_end_t = get_time(); \
	TRACE_LOG ("host-malloc end %lld\n", malloc_end_t); \
	*ptr = (void *) tmp_ptr; \
	if (level2_cond) { \
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time(); \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1, 0, &_gfn_status); \
		IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time(); \
		_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	IF_TIMING (_mm_overhead_time) insert_vtab_start_t = get_time(); \
	_insert_to_var_table(unique_id, *cl_ptr, mem_type, 1, (void *)tmp_ptr, NULL, NULL, NULL, NULL, NULL); \
	IF_TIMING (_mm_overhead_time) insert_vtab_end_t = get_time(); \
} while (0)

	
	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_1D(char,dim1_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_1D(unsigned char,dim1_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_1D(short,dim1_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_1D(unsigned short,dim1_size); break;
	case TYPE_INT:            SWITCH_MALLOC_1D(int,dim1_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_1D(unsigned,dim1_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_1D(long,dim1_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_1D(unsigned long,dim1_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_1D(float,dim1_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_1D(double,dim1_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_1D(long double,dim1_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_1D(long long int,dim1_size); break;
	}
	
#define SWITCH_MAP_BUFFER(type,size1) \
do { \
	*ptr = (double*) clEnqueueMapBuffer(_gfn_cmd_queue, *cl_ptr, CL_TRUE, map_type, 0, sizeof(type)*size1, 0,NULL, NULL, NULL); \
} while(0)

	if (_gfn_opt_level >= 3)
	{
		switch(type_id)
		{
		case TYPE_CHAR:           SWITCH_MAP_BUFFER(char,dim1_size); break;
		case TYPE_UNSIGNED_CHAR:  SWITCH_MAP_BUFFER(unsigned char,dim1_size); break;
		case TYPE_SHORT:          SWITCH_MAP_BUFFER(short,dim1_size); break;
		case TYPE_UNSIGNED_SHORT: SWITCH_MAP_BUFFER(unsigned short,dim1_size); break;
		case TYPE_INT:            SWITCH_MAP_BUFFER(int,dim1_size); break;
		case TYPE_UNSIGNED:       SWITCH_MAP_BUFFER(unsigned,dim1_size); break;
		case TYPE_LONG:           SWITCH_MAP_BUFFER(long,dim1_size); break;
		case TYPE_UNSIGNED_LONG:  SWITCH_MAP_BUFFER(unsigned long,dim1_size); break;
		case TYPE_FLOAT:          SWITCH_MAP_BUFFER(float,dim1_size); break;
		case TYPE_DOUBLE:         SWITCH_MAP_BUFFER(double,dim1_size); break;
		case TYPE_LONG_DOUBLE:    SWITCH_MAP_BUFFER(long double,dim1_size); break;
		case TYPE_LONG_LONG_INT:  SWITCH_MAP_BUFFER(long long int,dim1_size); break;
		}
	}

	IF_TIMING (_cluster_malloc_time && level1_cond)
		printf("[%d] Allocate %p on host : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(malloc_end_t-malloc_start_t)/1000000);
	IF_TIMING (_gpu_malloc_time && level2_cond)
		printf("[%d] Allocate %p on device : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);
	//IF_TIMING (_mm_overhead_time)
	
	return 0;
}

int _GfnMalloc2D(void *** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int level1_cond, int level2_cond)
{
	int i;
	long long malloc_start_t, malloc_end_t;
	long long create_buf_start_t, create_buf_end_t;
	long long insert_vtab_start_t, insert_vtab_end_t;

	int found = 0;
	int retieve_dim_num = 0;
	cl_mem_flags dummy_mem_type = 0;
	_retieve_var_table(unique_id, cl_ptr, &dummy_mem_type, &retieve_dim_num, (void**)ptr, &found);
	if (found) {
		//printf("Don't Allocate 2D Again\n");
		return 0;
	}

	if (dim1_size <= 0 && dim2_size <= 0) {
		fprintf(stdout, "ERROR %s:%d : Invalid size [%zu|%zu]\n", __FILE__, __LINE__, dim1_size, dim2_size);
	}

#define SWITCH_MALLOC_2D(type,size1,size2) \
do { \
	type ** tmp_ptr; \
	IF_TIMING (_cluster_malloc_time) malloc_start_t = get_time(); \
	TRACE_LOG ("host-malloc start %lld\n", malloc_start_t); \
	tmp_ptr = (type **) malloc(sizeof(type*) * size1); \
	tmp_ptr[0] = (type *) malloc(sizeof(type) * size1 * size2); \
	for (i = 1; i < size1; ++i) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	IF_TIMING (_cluster_malloc_time) malloc_end_t = get_time(); \
	TRACE_LOG ("host-malloc end %lld\n", malloc_end_t); \
	*ptr = (void **) tmp_ptr; \
	if (level2_cond) { \
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time(); \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2, 0, &_gfn_status); \
		IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time(); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	IF_TIMING (_mm_overhead_time) insert_vtab_start_t = get_time(); \
	_insert_to_var_table(unique_id, *cl_ptr, mem_type, 2, (void *)tmp_ptr[0], (void **)tmp_ptr, NULL, NULL, NULL, NULL); \
	IF_TIMING (_mm_overhead_time) insert_vtab_end_t = get_time(); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_2D(char,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_2D(unsigned char,dim1_size,dim2_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_2D(short,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_2D(unsigned short,dim1_size,dim2_size); break;
	case TYPE_INT:            SWITCH_MALLOC_2D(int,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_2D(unsigned,dim1_size,dim2_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_2D(long,dim1_size,dim2_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_2D(unsigned long,dim1_size,dim2_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_2D(float,dim1_size,dim2_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_2D(double,dim1_size,dim2_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_2D(long double,dim1_size,dim2_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_2D(long long int,dim1_size,dim2_size); break;
	}

	IF_TIMING (_cluster_malloc_time && level1_cond)
		printf("[%d] Allocate %p on host : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(malloc_end_t-malloc_start_t)/1000000);
	IF_TIMING (_gpu_malloc_time && level2_cond)
		printf("[%d] Allocate %p on device : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);
	//IF_TIMING (_mm_overhead_time)

	return 0;
}

int _GfnMalloc3D(void **** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int level1_cond, int level2_cond)
{
	int i;
	long long malloc_start_t, malloc_end_t;
	long long create_buf_start_t, create_buf_end_t;
	long long insert_vtab_start_t, insert_vtab_end_t;

	int found = 0;
	int retieve_dim_num = 0;
	cl_mem_flags dummy_mem_type = 0;
	_retieve_var_table(unique_id, cl_ptr, &dummy_mem_type, &retieve_dim_num, (void**)ptr, &found);
	if (found) {
		//printf("Don't Allocate 3D Again\n");
		return 0;
	}

	if (dim1_size <= 0 && dim2_size <= 0 && dim3_size <= 0) {
		fprintf(stdout, "ERROR %s:%d : Invalid size [%zu|%zu|%zu]\n", __FILE__, __LINE__, dim1_size, dim2_size, dim3_size);
	}

#define SWITCH_MALLOC_3D(type,size1,size2,size3) \
do { \
	type *** tmp_ptr; \
	IF_TIMING (_cluster_malloc_time) malloc_start_t = get_time(); \
	TRACE_LOG ("host-malloc start %lld\n", malloc_start_t); \
	tmp_ptr = (type ***) malloc(sizeof(type**) * size1); \
	tmp_ptr[0] = (type **) malloc(sizeof(type*) * size1 * size2); \
	for (i = 1; i < size1; i++) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	tmp_ptr[0][0] = (type *) malloc(sizeof(type) * size1 * size2 * size3); \
	for (i = 1; i < size1 * size2; i++) tmp_ptr[0][i] = tmp_ptr[0][i-1] + size3; \
	IF_TIMING (_cluster_malloc_time) malloc_end_t = get_time(); \
	TRACE_LOG ("host-malloc end %lld\n", malloc_end_t); \
	*ptr = (void ***) tmp_ptr; \
	if (level2_cond) { \
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time(); \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2 * size3, 0, &_gfn_status); \
    	IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time(); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	IF_TIMING (_mm_overhead_time) insert_vtab_start_t = get_time(); \
	_insert_to_var_table(unique_id, *cl_ptr, mem_type, 3, (void *)tmp_ptr[0][0], (void **)tmp_ptr[0], (void ***)tmp_ptr, NULL, NULL, NULL); \
	IF_TIMING (_mm_overhead_time) insert_vtab_end_t = get_time(); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_3D(char,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_3D(unsigned char,dim1_size,dim2_size,dim3_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_3D(short,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_3D(unsigned short,dim1_size,dim2_size,dim3_size); break;
	case TYPE_INT:            SWITCH_MALLOC_3D(int,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_3D(unsigned,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_3D(long,dim1_size,dim2_size,dim3_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_3D(unsigned long,dim1_size,dim2_size,dim3_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_3D(float,dim1_size,dim2_size,dim3_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_3D(double,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_3D(long double,dim1_size,dim2_size,dim3_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_3D(long long int,dim1_size,dim2_size,dim3_size); break;
	}

	IF_TIMING (_cluster_malloc_time && level1_cond)
		printf("[%d] Allocate %p on host : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(malloc_end_t-malloc_start_t)/1000000);
	IF_TIMING (_gpu_malloc_time && level2_cond)
		printf("[%d] Allocate %p on device : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);
	//IF_TIMING (_mm_overhead_time)

	return 0;
}

int _GfnMalloc4D(void ***** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int level1_cond, int level2_cond)
{
	int i;
	long long malloc_start_t, malloc_end_t;
	long long create_buf_start_t, create_buf_end_t;
	long long insert_vtab_start_t, insert_vtab_end_t;

	int found = 0;
	int retieve_dim_num = 0;
	cl_mem_flags dummy_mem_type = 0;
	_retieve_var_table(unique_id, cl_ptr, &dummy_mem_type, &retieve_dim_num, (void**)ptr, &found);
	if (found) {
		//printf("Don't Allocate 4D Again\n");
		return 0;
	}

	if (dim1_size <= 0 && dim2_size <= 0 && dim3_size <= 0 &&
		dim4_size <= 0) {
		fprintf(stdout, "ERROR %s:%d : Invalid size [%zu|%zu|%zu|%zu]\n", __FILE__, __LINE__, 
			dim1_size, dim2_size, dim3_size, dim4_size);
	}

#define SWITCH_MALLOC_4D(type,size1,size2,size3,size4) \
do { \
	type **** tmp_ptr; \
	IF_TIMING (_cluster_malloc_time) malloc_start_t = get_time(); \
	TRACE_LOG ("host-malloc start %lld\n", malloc_start_t); \
	tmp_ptr = (type ****) malloc(sizeof(type***) * size1); \
	tmp_ptr[0] = (type ***) malloc(sizeof(type**) * size1 * size2); \
	for (i = 1; i < size1; i++) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	tmp_ptr[0][0] = (type **) malloc(sizeof(type*) * size1 * size2 * size3); \
	for (i = 1; i < size1 * size2; i++) tmp_ptr[0][i] = tmp_ptr[0][i-1] + size3; \
	tmp_ptr[0][0][0] = (type *) malloc(sizeof(type) * size1 * size2 * size3 * size4); \
	for (i = 1; i < size1 * size2 * size3; i++) tmp_ptr[0][0][i] = tmp_ptr[0][0][i-1] + size4; \
	IF_TIMING (_cluster_malloc_time) malloc_end_t = get_time(); \
	TRACE_LOG ("host-malloc end %lld\n", malloc_end_t); \
	*ptr = (void ****) tmp_ptr; \
	if (level2_cond) { \
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time(); \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2 * size3 * size4, 0, &_gfn_status); \
    	IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time(); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	IF_TIMING (_mm_overhead_time) insert_vtab_start_t = get_time(); \
	_insert_to_var_table(unique_id, *cl_ptr, mem_type, 4, (void *)tmp_ptr[0][0][0], (void **)tmp_ptr[0][0], (void ***)tmp_ptr[0], (void ****)tmp_ptr, NULL, NULL); \
	IF_TIMING (_mm_overhead_time) insert_vtab_end_t = get_time(); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_4D(char,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_4D(unsigned char,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_4D(short,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_4D(unsigned short,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_INT:            SWITCH_MALLOC_4D(int,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_4D(unsigned,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_4D(long,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_4D(unsigned long,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_4D(float,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_4D(double,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_4D(long double,dim1_size,dim2_size,dim3_size,dim4_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_4D(long long int,dim1_size,dim2_size,dim3_size,dim4_size); break;
	}

	IF_TIMING (_cluster_malloc_time && level1_cond)
		printf("[%d] Allocate %p on host : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(malloc_end_t-malloc_start_t)/1000000);
	IF_TIMING (_gpu_malloc_time && level2_cond)
		printf("[%d] Allocate %p on device : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);
	//IF_TIMING (_mm_overhead_time)

	return 0;
}

int _GfnMalloc5D(void ****** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int level1_cond, int level2_cond)
{
	int i;
	long long malloc_start_t, malloc_end_t;
	long long create_buf_start_t, create_buf_end_t;
	long long insert_vtab_start_t, insert_vtab_end_t;

	int found = 0;
	int retieve_dim_num = 0;
	cl_mem_flags dummy_mem_type = 0;
	_retieve_var_table(unique_id, cl_ptr, &dummy_mem_type, &retieve_dim_num, (void**)ptr, &found);
	if (found) {
		//printf("Don't Allocate 5D Again\n");
		return 0;
	}

	if (dim1_size <= 0 && dim2_size <= 0 && dim3_size <= 0 &&
		dim4_size <= 0 && dim5_size <= 0) {
		fprintf(stdout, "ERROR %s:%d : Invalid size [%zu|%zu|%zu|%zu|%zu]\n", __FILE__, __LINE__, 
			dim1_size, dim2_size, dim3_size, dim4_size, dim5_size);
	}

#define SWITCH_MALLOC_5D(type,size1,size2,size3,size4,size5) \
do { \
	type ***** tmp_ptr; \
	IF_TIMING (_cluster_malloc_time) malloc_start_t = get_time(); \
	TRACE_LOG ("host-malloc start %lld\n", malloc_start_t); \
	tmp_ptr = (type *****) malloc(sizeof(type****) * size1); \
	tmp_ptr[0] = (type ****) malloc(sizeof(type***) * size1 * size2); \
	for (i = 1; i < size1; i++) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	tmp_ptr[0][0] = (type ***) malloc(sizeof(type**) * size1 * size2 * size3); \
	for (i = 1; i < size1 * size2; i++) tmp_ptr[0][i] = tmp_ptr[0][i-1] + size3; \
	tmp_ptr[0][0][0] = (type **) malloc(sizeof(type*) * size1 * size2 * size3 * size4); \
	for (i = 1; i < size1 * size2 * size3; i++) tmp_ptr[0][0][i] = tmp_ptr[0][0][i-1] + size4; \
	tmp_ptr[0][0][0][0] = (type *) malloc(sizeof(type) * size1 * size2 * size3 * size4 * size5); \
	for (i = 1; i < size1 * size2 * size3 * size4; i++) tmp_ptr[0][0][0][i] = tmp_ptr[0][0][0][i-1] + size5; \
	IF_TIMING (_cluster_malloc_time) malloc_end_t = get_time(); \
	TRACE_LOG ("host-malloc end %lld\n", malloc_end_t); \
	*ptr = (void *****) tmp_ptr; \
	if (level2_cond) { \
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time(); \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2 * size3 * size4 * size5, 0, &_gfn_status); \
    	IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time(); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	IF_TIMING (_mm_overhead_time) insert_vtab_start_t = get_time(); \
	_insert_to_var_table(unique_id, *cl_ptr, mem_type, 5, (void *)tmp_ptr[0][0][0][0], (void **)tmp_ptr[0][0][0], (void ***)tmp_ptr[0][0], (void ****)tmp_ptr[0], (void *****)tmp_ptr, NULL); \
	IF_TIMING (_mm_overhead_time) insert_vtab_end_t = get_time(); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_5D(char,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_5D(unsigned char,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_5D(short,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_5D(unsigned short,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_INT:            SWITCH_MALLOC_5D(int,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_5D(unsigned,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_5D(long,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_5D(unsigned long,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_5D(float,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_5D(double,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_5D(long double,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_5D(long long int,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size); break;
	}

	IF_TIMING (_cluster_malloc_time && level1_cond)
		printf("[%d] Allocate %p on host : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(malloc_end_t-malloc_start_t)/1000000);
	IF_TIMING (_gpu_malloc_time && level2_cond)
		printf("[%d] Allocate %p on device : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);
	//IF_TIMING (_mm_overhead_time)

	return 0;
}

int _GfnMalloc6D(void ******* ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int level1_cond, int level2_cond)
{ 
	int i;
	long long malloc_start_t, malloc_end_t;
	long long create_buf_start_t, create_buf_end_t;
	long long insert_vtab_start_t, insert_vtab_end_t;

	int found = 0;
	int retieve_dim_num = 0;
	cl_mem_flags dummy_mem_type = 0;
	_retieve_var_table(unique_id, cl_ptr, &dummy_mem_type, &retieve_dim_num, (void**)ptr, &found);
	if (found) {
		//printf("Don't Allocate 6D Again\n");
		return 0;
	}

	if (dim1_size <= 0 && dim2_size <= 0 && dim3_size <= 0 &&
		dim4_size <= 0 && dim5_size <= 0 && dim6_size <= 0) {
		fprintf(stdout, "ERROR %s:%d : Invalid size [%zu|%zu|%zu|%zu|%zu|%zu]\n", 
			__FILE__, __LINE__, dim1_size, dim2_size, 
			dim3_size, dim4_size, dim5_size, dim6_size);
	}

	// TODO: change to 1D array

#define SWITCH_MALLOC_6D(type,size1,size2,size3,size4,size5,size6) \
do { \
	type ****** tmp_ptr; \
	IF_TIMING (_cluster_malloc_time) malloc_start_t = get_time(); \
	TRACE_LOG ("host-malloc start %lld\n", malloc_start_t); \
	tmp_ptr = (type ******) malloc(sizeof(type*****) * size1); \
	tmp_ptr[0] = (type *****) malloc(sizeof(type****) * size1 * size2); \
	for (i = 1; i < size1; i++) tmp_ptr[i] = tmp_ptr[i-1] + size2; \
	tmp_ptr[0][0] = (type ****) malloc(sizeof(type***) * size1 * size2 * size3); \
	for (i = 1; i < size1 * size2; i++) tmp_ptr[0][i] = tmp_ptr[0][i-1] + size3; \
	tmp_ptr[0][0][0] = (type ***) malloc(sizeof(type**) * size1 * size2 * size3 * size4); \
	for (i = 1; i < size1 * size2 * size3; i++) tmp_ptr[0][0][i] = tmp_ptr[0][0][i-1] + size4; \
	tmp_ptr[0][0][0][0] = (type **) malloc(sizeof(type*) * size1 * size2 * size3 * size4 * size5); \
	for (i = 1; i < size1 * size2 * size3 * size4; i++) tmp_ptr[0][0][0][i] = tmp_ptr[0][0][0][i-1] + size5; \
	tmp_ptr[0][0][0][0][0] = (type *) malloc(sizeof(type) * size1 * size2 * size3 * size4 * size5 * size6); \
	for (i = 1; i < size1 * size2 * size3 * size4 * size5; i++) tmp_ptr[0][0][0][0][i] = tmp_ptr[0][0][0][0][i-1] + size6; \
	IF_TIMING (_cluster_malloc_time) malloc_end_t = get_time(); \
	TRACE_LOG ("host-malloc end %lld\n", malloc_end_t); \
	*ptr = (void ******) tmp_ptr; \
	if (level2_cond) { \
		IF_TIMING (_gpu_malloc_time) create_buf_start_t = get_time(); \
		*cl_ptr = clCreateBuffer(_gfn_context, mem_type, sizeof(type) * size1 * size2 * size3 * size4 * size5 * size6, 0, &_gfn_status); \
    	IF_TIMING (_gpu_malloc_time) create_buf_end_t = get_time(); \
    	_GfnCheckCLStatus(_gfn_status, "CREATE BUFFER"); \
	} \
	IF_TIMING (_mm_overhead_time) insert_vtab_start_t = get_time(); \
	_insert_to_var_table(unique_id, *cl_ptr, mem_type, 6, (void *)tmp_ptr[0][0][0][0][0], (void **)tmp_ptr[0][0][0][0], (void ***)tmp_ptr[0][0][0], (void ****)tmp_ptr[0][0], (void *****)tmp_ptr[0], (void ******)tmp_ptr); \
	IF_TIMING (_mm_overhead_time) insert_vtab_end_t = get_time(); \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_MALLOC_6D(char,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_MALLOC_6D(unsigned char,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_SHORT:          SWITCH_MALLOC_6D(short,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_MALLOC_6D(unsigned short,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_INT:            SWITCH_MALLOC_6D(int,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_UNSIGNED:       SWITCH_MALLOC_6D(unsigned,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_LONG:           SWITCH_MALLOC_6D(long,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_MALLOC_6D(unsigned long,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_FLOAT:          SWITCH_MALLOC_6D(float,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_DOUBLE:         SWITCH_MALLOC_6D(double,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_LONG_DOUBLE:    SWITCH_MALLOC_6D(long double,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	case TYPE_LONG_LONG_INT:  SWITCH_MALLOC_6D(long long int,dim1_size,dim2_size,dim3_size,dim4_size,dim5_size,dim6_size); break;
	}

	IF_TIMING (_cluster_malloc_time)
		printf("[%d] Allocate %p on host : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(malloc_end_t-malloc_start_t)/1000000);
	IF_TIMING (_gpu_malloc_time)
		printf("[%d] Allocate %p on device : %.10f s.\n", _gfn_rank, *ptr, 
			(float)(create_buf_end_t-create_buf_start_t)/1000000);
	//IF_TIMING (_mm_overhead_time)

	return 0;
}

int _GfnFree(long long unique_id, int level1_cond, int level2_cond)
{
	// Don't free now
	return 0;

	/*long long del_from_vtab_start_t, del_from_vtab_end_t;

	IF_TIMING (_mm_overhead_time) del_from_vtab_start_t = get_time();
	_free_mem_and_delete_from_var_table(unique_id);
	IF_TIMING (_mm_overhead_time) del_from_vtab_end_t = get_time();

	return 0;*/
}

int _GfnEnqueueBroadcastND(void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, int level1_cond, int level2_cond, int dim_n, ...)
{
	if (_is_lock_transfer(unique_id)) {
		return 0;
	}

	int chunk_size, chunk_num;
	size_t transfer_size, last_ite_size;
	cl_mem_flags mem_type;

	long long gpu_trans_start_t, gpu_trans_end_t;
	long long bcast_start_t, bcast_end_t;
	long long overall_bcast_start_t, overall_bcast_end_t;

	// TODO: make queue and broadcast out-of-order
	// TODO: level 2 transfer only used partition

	va_list vl;
	int found = 0;
	int i, size_array[MAX_DIM_SUPPORT];
	size_t total_size = 1;

	va_start(vl, dim_n);
	for (i = 0; i < dim_n; ++i) {
		size_array[i] = va_arg(vl, int);
		total_size *= size_array[i];
	}
	va_end(vl);

	// Not have information case
	if (dim_n == 0) {
		// TODO: get other information from var table
		_get_var_info(unique_id, &mem_type, &found);
		if (!found)
			fprintf(stdout, "ERROR %s:%d : cannot get variable info in _GfnEnqueueBroadcastND", __FILE__, __LINE__);
 	}

#define SWITCH_BCAST_ND(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	if (_gfn_rank == 0) _RecvInputMsg((void *)(tmp_ptr), sizeof(type) * total_size); \
	IF_TIMING (_cluster_bcast_time) bcast_start_t = get_time(); \
	TRACE_LOG ("node-transfer start %lld\n", bcast_start_t); \
	MPI_Bcast((void *)(tmp_ptr), total_size, mpi_type, 0, MPI_COMM_WORLD); \
	IF_TIMING (_cluster_bcast_time) bcast_end_t = get_time(); \
	TRACE_LOG ("node-transfer end %lld\n", bcast_end_t); \
	if (level2_cond) { \
		IF_TIMING (_gpu_transfer_h2d_time) gpu_trans_start_t = get_time(); \
		TRACE_LOG ("device-transfer start %lld\n", gpu_trans_start_t); \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * total_size, tmp_ptr, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
		IF_TIMING (_gpu_transfer_h2d_time) gpu_trans_end_t = get_time(); \
		TRACE_LOG ("device-transfer end %lld\n", gpu_trans_end_t); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_BCAST_ND(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_BCAST_ND(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_BCAST_ND(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_BCAST_ND(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_BCAST_ND(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_BCAST_ND(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_BCAST_ND(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_BCAST_ND(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_BCAST_ND(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_BCAST_ND(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_BCAST_ND(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_BCAST_ND(long long int,MPI_LONG_LONG_INT); break;
	}

	IF_TIMING (level2_cond && _gpu_transfer_h2d_time)
		printf("[%d] Transfer %p from host to device : %.10f s.\n", _gfn_rank, ptr, 
			(float)(gpu_trans_end_t-gpu_trans_start_t)/1000000);

	IF_TIMING (_cluster_bcast_time)
		printf("[%d] Broadcast %p : %.10f s.\n", _gfn_rank, ptr, 
			(float)(bcast_end_t-bcast_start_t)/1000000);

	return 0;
}

int _GfnEnqueueScatterND(void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ... )
{
	// send only bound data when locked
	int send_only_bound = 0;
	if (_is_lock_transfer(unique_id)) {
		send_only_bound = 1;

		if (pattern_n == 0)
			return 0;
	}

	int chunk_size, chunk_num;
	size_t transfer_size, last_ite_size;
	int subcnts[_gfn_num_proc];
	int subdisp[_gfn_num_proc];

	long long gpu_trans_start_t, gpu_trans_end_t;
	long long scatter_start_t, scatter_end_t;
	long long overall_scatter_start_t, overall_scatter_end_t;

	int i, k, r, value;
	va_list vl;
	int found = 0;

	int cnts[_gfn_num_proc];
	int disp[_gfn_num_proc];
	int sub_size, scatter_size;
	int recv_elem_offset = 0;
	int recv_it_offset = 0;		/* increase for (+=) (elem_num * block_size) */
	int recv_loop_num = 1, elem_num = 1, block_size = 1;
	int size_array[size_n], pattern_array[pattern_n];

	cl_buffer_region info;

	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	// Calculate block size and elements number
	for (i = 0; i < size_n; ++i) {
		if (i < partitioned_dim)         recv_loop_num *= size_array[i];
		else if (i == partitioned_dim)   elem_num      *= size_array[i];
		else /* i > partitioned_dim */   block_size    *= size_array[i];
	}
	scatter_size = elem_num * block_size;

	// FIXME: this quick fix for partition of loop and data isnot match
	if (partitioned_dim >= 0) {
		loop_start = 0;
		loop_end = size_array[partitioned_dim] - 1;
		loop_step = 1;
	}

	_CalcPartitionInfo(elem_num, block_size, loop_start, loop_end, loop_step, 
		pattern_array, pattern_n, pattern_type, cnts, disp);
	sub_size = cnts[_gfn_rank];
	recv_elem_offset = disp[_gfn_rank];

	// Not have information case
	if (size_n == 0) {
		// TODO: get other information from var table
		_get_var_info(unique_id, &mem_type, &found);
		if (!found)
			fprintf(stdout, "ERROR %s:%d : cannot get variable info in _GfnEnqueueScatterND", __FILE__, __LINE__);
	}

#ifdef OPTIMIZE_NO_USE_CL_SUBBUFFER
#define UPLOAD_TO_GPU(type) \
	_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * elem_num * block_size, tmp_ptr + recv_it_offset, 0, 0, 0); \
	_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER");
#else
#define UPLOAD_TO_GPU(type) \
	info.origin = (size_t)(recv_elem_offset * sizeof(type)); \
	info.size = (size_t)(sub_size * sizeof(type)); \
	cl_mem subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
	_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
	_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + recv_it_offset + recv_elem_offset, 0, 0, 0); \
	_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
	_gfn_status = clReleaseMemObject(subbuf); \
	_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER");
#endif

#define SWITCH_SCATTER_ND(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	if (_gfn_rank == 0) { \
		_RecvInputNDMsgCore(tmp_ptr,type_id,loop_start,loop_end,loop_step, \
						partitioned_dim,pattern_type,size_n,pattern_n,size_array,pattern_array); \
	} \
for (i = 0; i < recv_loop_num; ++i) { \
	IF_TIMING (_cluster_scatter_time) scatter_start_t = get_time(); \
	TRACE_LOG ("node-transfer start %lld\n", scatter_start_t); \
	MPI_Scatterv(tmp_ptr + recv_it_offset, cnts, disp, mpi_type, \
		tmp_ptr + recv_it_offset + recv_elem_offset, sub_size, mpi_type, 0, MPI_COMM_WORLD); \
	IF_TIMING (_cluster_scatter_time) scatter_end_t = get_time(); \
	TRACE_LOG ("node-transfer end %lld\n", scatter_end_t); \
	if (level2_cond) { \
		IF_TIMING (_gpu_transfer_h2d_time) gpu_trans_start_t = get_time(); \
		TRACE_LOG ("device-transfer start %lld\n", gpu_trans_start_t); \
		UPLOAD_TO_GPU(type) \
		IF_TIMING (_gpu_transfer_h2d_time) gpu_trans_end_t = get_time(); \
		TRACE_LOG ("device-transfer end %lld\n", gpu_trans_end_t); \
	} \
	recv_it_offset += (elem_num * block_size); \
} \
} while (0)

#define SEND_LOWER_BOUND_TAG 0
#define SEND_UPPER_BOUMD_TAG 1

	// Send only bound case
	if (send_only_bound)
	{
		double * tmp_ptr = (double *) ptr;

		cl_buffer_region send_lower_info, send_upper_info;
		cl_buffer_region recv_lower_info, recv_upper_info;
		cl_mem cl_send_lower, cl_send_upper;
		cl_mem cl_recv_lower, cl_recv_upper;
		MPI_Status mpi_status;
		MPI_Request send_lower_req, send_upper_req;
		MPI_Request recv_lower_req, recv_upper_req;

		int lower_bound = pattern_array[0];
    	int upper_bound = pattern_array[1];
		int lower_bound_size = abs(lower_bound) * block_size;
		int upper_bound_size = abs(upper_bound) * block_size;

		int type_size = _CalcTypeSize(type_id);
		MPI_Datatype mpi_type = _GetMpiDatatype(type_id);

#ifdef DEBUG_BOUND
		//printf("[DEBUG BOUND]: Send only bound in _GfnEnqueueScatterND\n");
		//printf("[DEBUG BOUND]: Lower bound is %d\n", lower_bound);
		//printf("[DEBUG BOUND]: Upper bound is %d\n", upper_bound);
#endif

		// FIXME : remove this constraint
		if (lower_bound_size != upper_bound_size)
			fprintf(stderr, "ERROR %s:%d : in pattern lower and upper bound are not equal,"
				" lower bound size is %d and upper bound size is %d\n",
				__FILE__, __LINE__, lower_bound, upper_bound);

	if (level2_cond) {
		// create send lower bound subbuffer
		if (_gfn_rank != 0 && lower_bound_size > 0) {
			send_lower_info.origin = (size_t)(disp[_gfn_rank]*type_size);
			send_lower_info.size = (size_t)(lower_bound_size*type_size);
			cl_send_lower = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &send_lower_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE SEND LOWER BOUND");
		}
		// create send upper bound subbuffer
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0) {
			send_upper_info.origin = (size_t)((disp[_gfn_rank+1]-upper_bound_size)*type_size);
			send_upper_info.size = (size_t)(upper_bound_size*type_size);
			cl_send_upper = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &send_upper_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE SEND UPPER BOUND");
		}
	
		// download send lower bound subbuffer from GPU to host
		if (_gfn_rank != 0 && lower_bound_size > 0) {
			_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, cl_send_lower, CL_FALSE,
				0, lower_bound_size*type_size, tmp_ptr+disp[_gfn_rank],
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "DOWNLOAD SEND LOWER BOUND FROM DEVICE");
			
		}
		// download send upper bound subbuffer from GPU to host
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0) {
			_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, cl_send_upper, CL_FALSE,
				0, upper_bound_size*type_size, tmp_ptr+disp[_gfn_rank+1]-upper_bound_size,
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "DOWNLOAD SEND UPPER BOUND FROM DEVICE");
		}
		_gfn_status = clFinish(_gfn_cmd_queue);
	}


		// send lower bound asynchonously
		if (_gfn_rank != 0 && lower_bound_size > 0) {
			MPI_Isend((void*)(tmp_ptr+disp[_gfn_rank]), lower_bound_size, 
				mpi_type, _gfn_rank-1, SEND_LOWER_BOUND_TAG, MPI_COMM_WORLD, &send_lower_req);
		}
		// send upper bound asynchonously
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0) {
			MPI_Isend((void*)(tmp_ptr+disp[_gfn_rank+1]-upper_bound_size), upper_bound_size, 
				mpi_type, _gfn_rank+1, SEND_UPPER_BOUMD_TAG, MPI_COMM_WORLD, &send_upper_req);
		}
		// recieve lower bound asynchonously
		if (_gfn_rank != 0 && upper_bound_size > 0) {
			MPI_Irecv((void*)(tmp_ptr+disp[_gfn_rank]-upper_bound_size), upper_bound_size,
				mpi_type, _gfn_rank-1, SEND_UPPER_BOUMD_TAG, MPI_COMM_WORLD, &recv_lower_req);
		}
		// recieve upper bound asynchonously
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0) {
			MPI_Irecv((void*)(tmp_ptr+disp[_gfn_rank+1]), lower_bound_size, 
				mpi_type, _gfn_rank+1, SEND_LOWER_BOUND_TAG, MPI_COMM_WORLD, &recv_upper_req);
		}


		// synchonize recieve
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0)
			MPI_Wait(&send_upper_req, &mpi_status);
		if (_gfn_rank != 0 && lower_bound_size > 0)
			MPI_Wait(&send_lower_req, &mpi_status);
		if (_gfn_rank != 0 && upper_bound_size > 0)
			MPI_Wait(&recv_lower_req, &mpi_status);
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0)
			MPI_Wait(&recv_upper_req, &mpi_status);


	if (level2_cond) {
		// create recieve lower bound subbuffer
		if (_gfn_rank != 0 && upper_bound_size > 0) {
			recv_lower_info.origin = (size_t)((disp[_gfn_rank]-upper_bound_size)*type_size);
			recv_lower_info.size = (size_t)(upper_bound_size*type_size);
			cl_recv_lower = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &recv_lower_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE RECIEVE LOWER BOUND");
		}
		// create recieve upper bound subbuffer
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0) {
			recv_upper_info.origin = (size_t)(disp[_gfn_rank+1]*type_size);
			recv_upper_info.size = (size_t)(lower_bound_size*type_size);
			cl_recv_upper = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &recv_upper_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE RECIEVE UPPER BOUND");
		}

		// upload upper bound to device
		if (_gfn_rank != 0 && upper_bound_size > 0) {
			_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_recv_lower, CL_FALSE, 
				0, upper_bound_size*sizeof(double), (tmp_ptr+disp[_gfn_rank]-upper_bound_size),
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "UPLOAD RECIEVE UPPER BOUND FROM HOST");
		}
		// upload lower bound to device
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0) {
			_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_recv_upper, CL_FALSE, 
				0, lower_bound_size*sizeof(double), (tmp_ptr+disp[_gfn_rank+1]),
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "UPLOAD RECIEVE LOWER BOUND FROM HOST");
		}
		_gfn_status = clFinish(_gfn_cmd_queue);
	}

		return 0;
	}

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_SCATTER_ND(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_SCATTER_ND(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_SCATTER_ND(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_SCATTER_ND(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_SCATTER_ND(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_SCATTER_ND(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_SCATTER_ND(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_SCATTER_ND(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_SCATTER_ND(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_SCATTER_ND(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_SCATTER_ND(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_SCATTER_ND(long long int,MPI_LONG_LONG_INT); break;
	}

	IF_TIMING (_cluster_scatter_time)
		printf("[%d] Scatter %p : %.10f s.\n", _gfn_rank, ptr, 
			(float)(scatter_end_t-scatter_start_t)/1000000);

	IF_TIMING (level2_cond && _gpu_transfer_h2d_time)
		printf("[%d] Transfer %p from host to device : %.10f s.\n", _gfn_rank, ptr, 
			(float)(gpu_trans_end_t-gpu_trans_start_t)/1000000);

	return 0;
}

int _GfnFinishDistributeArray()
{
	return 0;
}

int _GfnEnqueueGatherND(void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, int level1_cond, int level2_cond, int size_n, int pattern_n, ... )
{
	int send_only_bound = 0;
	if (_is_lock_transfer(unique_id)) {
		//printf("gather only bound\n");
		send_only_bound = 1;

		if (pattern_n == 0)
			return 0;
	}

	long long gpu_trans_start_t, gpu_trans_end_t;
	long long gather_start_t, gather_end_t;

	int i, value;
	va_list vl;

	int cnts[_gfn_num_proc];
	int disp[_gfn_num_proc];
	int sub_size;
	int send_elem_offset = 0;
	int send_it_offset = 0;		/* increase for (+=) (elem_num * block_size) */
	int send_loop_num = 1, elem_num = 1, block_size = 1;
	int size_array[size_n], pattern_array[pattern_n];

	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	// Calculate block size and elements number
	for (i = 0; i < size_n; ++i) {
		if (i < partitioned_dim)         send_loop_num *= size_array[i];
		else if (i == partitioned_dim)   elem_num      *= size_array[i];
		else /* i > partitioned_dim */   block_size    *= size_array[i];
	}

	// FIXME: this quick fix for partition of loop and data isnot match
	if (partitioned_dim >= 0) {
		loop_start = 0;
		loop_end = size_array[partitioned_dim] - 1;
		loop_step = 1;
	}

	_CalcPartitionInfo(elem_num, block_size, loop_start, loop_end, loop_step, 
		pattern_array, pattern_n, pattern_type, cnts, disp);
	sub_size = cnts[_gfn_rank];
	send_elem_offset = disp[_gfn_rank];

#ifdef OPTIMIZE_NO_USE_CL_SUBBUFFER
#define DOWNLOAD_FROM_GPU(type) \
	_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, cl_ptr, CL_TRUE, 0, sizeof(type) * elem_num * block_size, tmp_ptr + send_it_offset, 0, 0, 0); \
    _GfnCheckCLStatus(_gfn_status, "READ BUFFER");
#else
#define DOWNLOAD_FROM_GPU(type) \
	cl_buffer_region info; \
	info.origin = (size_t)(send_elem_offset * sizeof(type)); \
	info.size = (size_t)(sub_size * sizeof(type)); \
	cl_mem subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
	_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
	_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + send_it_offset + send_elem_offset, 0, 0, 0); \
    _GfnCheckCLStatus(_gfn_status, "READ BUFFER"); \
    _gfn_status = clReleaseMemObject(subbuf); \
	_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER");
#endif

#define SWITCH_GATHER_ND(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
for (i = 0; i < send_loop_num; ++i) { \
	if (level2_cond) { \
		IF_TIMING (_gpu_transfer_h2d_time) gpu_trans_start_t = get_time(); \
		TRACE_LOG ("device-transfer start %lld\n", gpu_trans_start_t); \
		DOWNLOAD_FROM_GPU(type) \
		IF_TIMING (_gpu_transfer_h2d_time) gpu_trans_end_t = get_time(); \
		TRACE_LOG ("device-transfer end %lld\n", gpu_trans_end_t); \
	} \
	IF_TIMING (_cluster_gather_time) gather_start_t = get_time(); \
	TRACE_LOG ("node-transfer start %lld\n", gather_start_t); \
	if (_gfn_rank == 0) \
		MPI_Gatherv(MPI_IN_PLACE, sub_size, mpi_type, \
			tmp_ptr + send_it_offset, cnts, disp, mpi_type, 0, MPI_COMM_WORLD); \
	else \
		MPI_Gatherv(tmp_ptr + send_it_offset + send_elem_offset, sub_size, mpi_type, \
			tmp_ptr + send_it_offset, cnts, disp, mpi_type, 0, MPI_COMM_WORLD); \
	IF_TIMING (_cluster_gather_time) gather_end_t = get_time(); \
	TRACE_LOG ("node-transfer end %lld\n", gather_end_t); \
	send_it_offset += (elem_num * block_size); \
} \
	if (_gfn_rank == 0) { \
		_SendOutputNDMsgCore(tmp_ptr,type_id,loop_start,loop_end,loop_step, \
						 partitioned_dim,pattern_type,size_n,pattern_n,size_array,pattern_array); \
	} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_GATHER_ND(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_GATHER_ND(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_GATHER_ND(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_GATHER_ND(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_GATHER_ND(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_GATHER_ND(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_GATHER_ND(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_GATHER_ND(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_GATHER_ND(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_GATHER_ND(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_GATHER_ND(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_GATHER_ND(long long int,MPI_LONG_LONG_INT); break;
	}

	IF_TIMING (level2_cond && _gpu_transfer_h2d_time)
		printf("[%d] Transfer %p from device to host : %.10f s.\n", _gfn_rank, ptr, 
			(float)(gpu_trans_end_t-gpu_trans_start_t)/1000000);

	IF_TIMING (_cluster_gather_time)
		printf("[%d] Gather %p : %.10f s.\n", _gfn_rank, ptr, 
			(float)(gather_end_t-gather_start_t)/1000000);

	return 0;
}

int _GfnFinishGatherArray()
{
	return 0;
}


// Stream sequence
int _GfnStreamSeqKernelRegister(long long kernel_id, int local_start, int local_end, int loop_start, int loop_end, int loop_step, struct _kernel_information **out_ker_info)
{
	int found = 0;
	struct _kernel_information *ker_info;
	
	_retieve_kernel_table(kernel_id, &ker_info, &found);
	
	if (!found) ker_info = (struct _kernel_information *)malloc(sizeof(struct _kernel_information));
	
	ker_info->kernel_id = kernel_id;
	ker_info->local_start = local_start;
	ker_info->local_end = local_end;
	ker_info->loop_start = loop_start;
	ker_info->loop_end = loop_end;
	ker_info->loop_step = loop_step;
	
    //printf("RANK[%d] LOOP (%d,%d,%d)\n", _gfn_rank, ker_info->local_start,
      //  ker_info->local_end, ker_info->loop_step);
    
	ker_info->curr_sequence_id = 0;
	
	ker_info->last_exec_partition_size = 0;
	ker_info->last_upload_seq_start = 0;
	ker_info->last_upload_seq_end = 0;
	ker_info->last_upload_partition_size = 0;
	ker_info->last_partition_seq_start = 0;
	ker_info->last_partition_seq_end = 0;
	ker_info->last_partition_partition_size = 0;
	
	ker_info->bcast_var_num = 0;
	ker_info->scatter_var_num = 0;
	ker_info->gather_var_num = 0;
	
	ker_info->has_exec_evt = 0;
	
	if (!found) _insert_to_kernel_table(kernel_id, ker_info);
	
	*out_ker_info = ker_info;
}

int _GfnStreamSeqKernelGetNextSequence(struct _kernel_information *ker_info, int *seq_id, 
                                       size_t *stream_global_item_num, size_t *stream_work_group_item_num)
{
	size_t var_num, gather_var_num;
	int vit, i;
	struct _data_information ** data_infos;
	struct _data_information ** gather_data_infos;
	_get_scatter_var_ids(ker_info, &data_infos, &var_num);
	_get_scatter_var_ids(ker_info, &gather_data_infos, &gather_var_num);
	MPI_Status status;
	
	if (_debug_stream_seq && _gfn_rank == 0) {
		printf("SS: SEQUENCE NUMBER : %d\n", ker_info->curr_sequence_id);
	}
	
	// TODO: calculate partition size dynamically
	//int curr_ite_partition_size = 200000;
	int curr_ite_partition_size = _CalcLoopSize(ker_info->loop_start, ker_info->loop_end, ker_info->loop_step) / (_gfn_num_proc * stream_seq_factor);

	
	// ONLY BROADCAST AND SCATTER DATA
	if (ker_info->curr_sequence_id == 0) {
		// get sequence start and end index
		ker_info->last_partition_seq_start = ker_info->local_start;
		ker_info->last_partition_partition_size = curr_ite_partition_size;
		ker_info->last_partition_seq_end = ker_info->last_partition_seq_start + ker_info->last_partition_partition_size;
		if (ker_info->last_partition_seq_end > ker_info->local_end)
			ker_info->last_partition_seq_end = ker_info->local_end;
		
		ker_info->stream_seq_start_idx = -1;
		ker_info->stream_seq_end_idx = -1;
        
		for (vit = 0; vit < var_num; ++vit) {
			struct _data_information * data_info = data_infos[vit];
			
			/*if (_debug_stream_seq && _gfn_rank == 0) {
				printf("last_partition_partition_size = %d\n", ker_info->last_partition_partition_size);
				printf("last_partition_seq_start = %d\n", ker_info->last_partition_seq_start);
				printf("last_partition_seq_end = %d\n", ker_info->last_partition_seq_end);
			}*/
			
			// Initialize cnts for first partition
			for (i = 0; i < _gfn_num_proc; ++i) {
				if (data_info->last_partition_cnts[i] > ker_info->last_partition_partition_size) {
					data_info->last_partition_cnts[i] = ker_info->last_partition_partition_size/* * data_info->block_size*/;
				}
			}
			
			if (_debug_stream_seq && _gfn_rank == 0) {
				int r;
				for (r = 0; r < _gfn_num_proc; ++r)
					printf("SS: PARTITION CNTS[%d] = %d , DISP[%d] = %d, END[%d] = %d\n", 
					       r, data_info->last_partition_cnts[r], r, data_info->last_partition_disp[r],
                           r, data_info->end_disp[r] );
			}
			_GfnStreamSeqIScatter(data_info);
		}
	} else {
		_update_kernel_info_seq(ker_info);
		
		// get sequence start and end index
		ker_info->last_partition_seq_start += ker_info->last_partition_partition_size;
		// ker_info->last_partition_partition_size = 1000;
		ker_info->last_partition_seq_end = ker_info->last_partition_seq_start + ker_info->last_partition_partition_size;
		if (ker_info->last_partition_seq_end > ker_info->local_end)
			ker_info->last_partition_seq_end = ker_info->local_end;
		
		ker_info->stream_seq_start_idx = (ker_info->stream_seq_start_idx < 0)? ker_info->local_start : ker_info->stream_seq_end_idx + 1;
		ker_info->stream_seq_end_idx = ker_info->stream_seq_start_idx + ker_info->last_exec_partition_size - 1;
		if (ker_info->stream_seq_end_idx > ker_info->local_end)
			ker_info->stream_seq_end_idx = ker_info->local_end;
		
		// 2. for variable 2 - last
		for (vit = 0; vit < var_num; ++vit) {
			struct _data_information * data_info = data_infos[vit];
			
			// Update cnts and disp
			for (i = 0; i < _gfn_num_proc; ++i) {
				data_info->last_upload_cnts[i] = data_info->last_partition_cnts[i];
				data_info->last_upload_disp[i] = data_info->last_partition_disp[i];

				if (data_info->last_partition_disp[i] < 0) continue;
                
				data_info->last_partition_disp[i] = data_info->last_partition_disp[i] + data_info->last_partition_cnts[i];
				data_info->last_partition_cnts[i] = ker_info->last_partition_partition_size/* * data_info->block_size*/;
				if (data_info->last_partition_disp[i] >= data_info->end_disp[i])
				{
					data_info->last_partition_cnts[i] = -1;
					data_info->last_partition_disp[i] = -1;
				}
				else if (data_info->last_partition_disp[i] + data_info->last_partition_cnts[i]  > data_info->end_disp[i])
				{
					data_info->last_partition_cnts[i] = data_info->end_disp[i] - data_info->last_partition_disp[i];
				}
			}
			
			// 2.1 IScatter (part I+1)
			if (_debug_stream_seq && _gfn_rank == 0) {
				int r;
				for (r = 0; r < _gfn_num_proc; ++r)
					printf("SS: PARTITION CNTS[%d] = %d , DISP[%d] = %d\n", 
					       r, data_info->last_partition_cnts[r], r, data_info->last_partition_disp[r]);
			}
			if (data_info->has_iscatter_req) MPI_Wait(&(data_info->last_iscatter_req), &status);
			_GfnStreamSeqIScatter(data_info);
		}
        
		for (vit = 0; vit < var_num; ++vit) {
			struct _data_information * data_info = data_infos[vit];
            
			// 2.2 WriteBuffer (part I)
			if (_debug_stream_seq && _gfn_rank == 0) {
				int r;
				for (r = 0; r < _gfn_num_proc; ++r)
					printf("SS: UPLOAD CNTS[%d] = %d , DISP[%d] = %d\n", 
					       r, data_info->last_upload_cnts[r], r, data_info->last_upload_disp[r]);
			}
			if (data_info->has_upload_evt) clWaitForEvents(1, &(data_info->last_upload_evt));
			_GfnStreamSeqWriteBuffer(data_info);
		}
		
		if (_debug_stream_seq && ker_info->curr_sequence_id >= 2) {
			printf("SS: EXECUTION START[%d] = %d , END = %d\n", _gfn_rank, ker_info->stream_seq_start_idx, ker_info->stream_seq_end_idx);
		}
	}
	
	size_t _work_item_num = (size_t)_CalcLoopSize(ker_info->stream_seq_start_idx, ker_info->stream_seq_end_idx, ker_info->loop_step);
	*stream_work_group_item_num = 64; // TODO: optimization this value
	*stream_global_item_num = _GfnCalcGlobalItemNum(_work_item_num, *stream_work_group_item_num);
	
	ker_info->has_exec_evt = 0; // set to 1 if execute
	*seq_id = ker_info->curr_sequence_id; // seq_id is output variable
	
	ker_info->is_complete = 0;
	
	return 0;
}

int _GfnStreamSeqKernelFinishSequence(struct _kernel_information *ker_info)
{
	size_t var_num;
	int vit, i;
	int has_gather = 1;
	struct _data_information ** data_infos;
	_get_gather_var_ids(ker_info, &data_infos, &var_num);
	MPI_Status status;
	
	if (ker_info->curr_sequence_id == 0) {
		// Initialize gather variable information
		for (vit = 0; vit < var_num; ++vit) {
			struct _data_information * data_info = data_infos[vit];
			for (i = 0; i < _gfn_num_proc; ++i) {
				if (data_info->last_partition_cnts[i] > ker_info->last_partition_partition_size) {
					data_info->last_partition_cnts[i] = ker_info->last_partition_partition_size/* * data_info->block_size*/;
				}
				
				data_info->last_upload_cnts[i] = -1;
				data_info->last_upload_disp[i] = -1;
				
				data_info->last_exec_cnts[i] = -1;
				data_info->last_exec_disp[i] = -1;
				
				data_info->last_download_cnts[i] = -1;
				data_info->last_download_disp[i] = -1;
				
				data_info->last_gather_cnts[i] = -1;
				data_info->last_gather_disp[i] = -1;
			}
		}
	}
	else {
		// update gather variable information
		for (vit = 0; vit < var_num; ++vit) {
			struct _data_information * data_info = data_infos[vit];		
			for (i = 0; i < _gfn_num_proc; ++i) {
				// new gather counts
				data_info->last_gather_cnts[i] = data_info->last_download_cnts[i];
				data_info->last_gather_disp[i] = data_info->last_download_disp[i];
				if (data_info->last_gather_cnts[i] == -1) has_gather = 0;
				
				// new download counts
				data_info->last_download_cnts[i] = data_info->last_exec_cnts[i];
				data_info->last_download_disp[i] = data_info->last_exec_disp[i];
				
				// new exec counts
				data_info->last_exec_cnts[i] = data_info->last_upload_cnts[i];
				data_info->last_exec_disp[i] = data_info->last_upload_disp[i];
				
				// new upload counts
				data_info->last_upload_cnts[i] = data_info->last_partition_cnts[i];
				data_info->last_upload_disp[i] = data_info->last_partition_disp[i];

				// new partition counts
				if (data_info->last_partition_disp[i] < 0) continue;
				data_info->last_partition_disp[i] = data_info->last_partition_disp[i] + data_info->last_partition_cnts[i];
				data_info->last_partition_cnts[i] = ker_info->last_partition_partition_size/* * data_info->block_size*/;
				
				if (data_info->last_partition_disp[i] >= data_info->end_disp[i])
				{
					data_info->last_partition_cnts[i] = -1;
					data_info->last_partition_disp[i] = -1;
				}
				else if (data_info->last_partition_disp[i] + data_info->last_partition_cnts[i]  > data_info->end_disp[i])
				{
					data_info->last_partition_cnts[i] = data_info->end_disp[i] - data_info->last_partition_disp[i];
				}
			}
		}
	}
	
	for (vit = 0; vit < var_num; ++vit) {
		struct _data_information * data_info = data_infos[vit];
		
		if (_debug_stream_seq && _gfn_rank == 0) {
			int r;
			for (r = 0; r < _gfn_num_proc; ++r)
				printf("SS: GATHER CNTS[%d] = %d , DISP[%d] = %d\n", 
				       r, data_info->last_gather_cnts[r], r, data_info->last_gather_disp[r]);
		}
		if (data_info->has_igather_req) MPI_Wait(&(data_info->last_igather_req), &status);
		_GfnStreamSeqIGather(data_info);
	}
	
	for (vit = 0; vit < var_num; ++vit) {
		struct _data_information * data_info = data_infos[vit];

		if (_debug_stream_seq && _gfn_rank == 0) {
			int r;
			for (r = 0; r < _gfn_num_proc; ++r)
				printf("SS: READ CNTS[%d] = %d , DISP[%d] = %d\n", 
				       r, data_info->last_download_cnts[r], r, data_info->last_download_disp[r]);
		}
		//if (data_info->has_download_evt) clWaitForEvents(1, &(data_info->last_download_evt));
		_GfnStreamSeqReadBuffer(data_info);
	}
	
	if (_debug_stream_seq && _gfn_rank == 0) {
		printf("========================================\n\n");
	}
	
	// wait for execution
	// TODO: move to before next conputation
	if (ker_info->has_exec_evt) clWaitForEvents(1, &(ker_info->exec_evt));
	
	//printf("stream %d:%d\n", ker_info->stream_seq_start_idx, ker_info->stream_seq_end_idx);
	//printf("stream = %d\n", _GfnStreamSeqExec(ker_info->stream_seq_start_idx, ker_info->stream_seq_end_idx));
	
	if (ker_info->curr_sequence_id > 2 &&
		_GfnStreamSeqExec(ker_info->stream_seq_start_idx, ker_info->stream_seq_end_idx) == 0 
		&& has_gather == 0) {
		ker_info->is_complete = 1;
	}
	
	// last iteration block for all download and gather
	if (ker_info->is_complete) {
		for (vit = 0; vit < var_num; ++vit) {
		struct _data_information * data_info = data_infos[vit];
		if (data_info->has_igather_req) MPI_Wait(&(data_info->last_igather_req), &status);
	}
	}
	
	// update kernel info
	ker_info->curr_sequence_id += 1;
}

int _GfnStreamSeqEnqueueScatterND(struct _kernel_information *ker_info, void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ... )
{
	if (_is_lock_transfer(unique_id)) {
		//send_only_bound = 1;
		if (pattern_n == 0) // no dependency
			return 0;
	}
  
	int scatter_size;
	int recv_it_offset = 0;		/* increase for (+=) (elem_num * block_size) */
	int recv_loop_num = 1;
	int size_array[size_n], pattern_array[pattern_n];
	struct _data_information *data_info = (struct _data_information *) malloc(sizeof(struct _data_information));
	_init_data_info(data_info);

	int i, found = 0;
	va_list vl;
	
	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);
	_set_data_info_pattern(data_info, pattern_type, pattern_n, pattern_array);

	// Calculate block size and elements number
	data_info->elem_num = 1;
	data_info->block_size = 1;
	for (i = 0; i < size_n; ++i) {
		if (i < partitioned_dim)         recv_loop_num *= size_array[i];
		else if (i == partitioned_dim)   data_info->elem_num    *= size_array[i];
		else /* i > partitioned_dim */   data_info->block_size  *= size_array[i];
	}
	
	if (_debug_stream_seq && _gfn_rank == 0) {
		printf("ELEMENT NUM = %zu\n", data_info->elem_num);
		printf("BLOCK SIZE = %zu\n", data_info->block_size);
	}

	// FIXME: this quick fix for partition of loop and data isnot match
	if (partitioned_dim >= 0) {
		loop_start = 0;
		loop_end = size_array[partitioned_dim] - 1;
		loop_step = 1;
	}
	_set_data_info_loop(data_info, loop_start, loop_end, loop_step);

	// Not have information case
	if (size_n == 0) {
		// TODO: get other information from var table
		_get_var_info(unique_id, &mem_type, &found);
		if (!found)
			fprintf(stdout, "ERROR %s:%d : cannot get variable info in _GfnEnqueueScatterND", __FILE__, __LINE__);
	}
	
	_set_data_info_variable(data_info, unique_id, ptr, cl_ptr, mem_type, type_id);

	// Send data from worker root to others
	switch(type_id)
	{
#define SWITCH_STREAMSEQ_MASTER_SEND(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	if (_gfn_rank == 0) { \
		_RecvInputNDMsgCore(tmp_ptr,type_id,loop_start,loop_end,loop_step, \
			partitioned_dim,pattern_type,size_n,pattern_n,size_array,pattern_array); \
	} \
} while (0)
	case TYPE_CHAR:           SWITCH_STREAMSEQ_MASTER_SEND(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAMSEQ_MASTER_SEND(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAMSEQ_MASTER_SEND(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAMSEQ_MASTER_SEND(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAMSEQ_MASTER_SEND(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAMSEQ_MASTER_SEND(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAMSEQ_MASTER_SEND(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAMSEQ_MASTER_SEND(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAMSEQ_MASTER_SEND(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAMSEQ_MASTER_SEND(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAMSEQ_MASTER_SEND(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAMSEQ_MASTER_SEND(long long int,MPI_LONG_LONG_INT); break;
	}
	
	_add_scatter_var_id( ker_info, data_info ); // Register to kernel
	
	// Calculate disp (cnts is depend on partition size)
	_CalcPartitionInfo(
		data_info->elem_num, 
		data_info->block_size, 
		data_info->loop_start,
		data_info->loop_end, 
		data_info->loop_step, 
		data_info->pattern_array, 
		data_info->pattern_num, 
		data_info->pattern_type, 
		data_info->last_partition_cnts, /* OUTPUT */
		data_info->last_partition_disp  /* OUTPUT */
	);
	for (i = 0; i < _gfn_num_proc; ++i)
		data_info->end_disp[i] = data_info->last_partition_disp[i] + data_info->last_partition_cnts[i];
	
#if 0

	/* Exchange boundary if stream number is -1 */
	if (stream_no == 0) {
#define GET_ARRAY_FROM_MASTER(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	if (_gfn_rank == 0) { \
		_RecvInputNDMsgCore(tmp_ptr,type_id,loop_start,loop_end,loop_step, \
						partitioned_dim,pattern_type,size_n,pattern_n,size_array,pattern_array); \
	} \
} while(0)

		switch(type_id)
		{
		case TYPE_CHAR:           GET_ARRAY_FROM_MASTER(char,MPI_CHAR); break;
		case TYPE_UNSIGNED_CHAR:  GET_ARRAY_FROM_MASTER(unsigned char,MPI_UNSIGNED_CHAR); break;
		case TYPE_SHORT:          GET_ARRAY_FROM_MASTER(short,MPI_SHORT); break;
		case TYPE_UNSIGNED_SHORT: GET_ARRAY_FROM_MASTER(unsigned short,MPI_UNSIGNED_SHORT); break;
		case TYPE_INT:            GET_ARRAY_FROM_MASTER(int,MPI_INT); break;
		case TYPE_UNSIGNED:       GET_ARRAY_FROM_MASTER(unsigned,MPI_UNSIGNED); break;
		case TYPE_LONG:           GET_ARRAY_FROM_MASTER(long,MPI_LONG); break;
		case TYPE_UNSIGNED_LONG:  GET_ARRAY_FROM_MASTER(unsigned long,MPI_UNSIGNED_LONG); break;
		case TYPE_FLOAT:          GET_ARRAY_FROM_MASTER(float,MPI_FLOAT); break;
		case TYPE_DOUBLE:         GET_ARRAY_FROM_MASTER(double,MPI_DOUBLE); break;
		case TYPE_LONG_DOUBLE:    GET_ARRAY_FROM_MASTER(long double,MPI_LONG_DOUBLE); break;
		case TYPE_LONG_LONG_INT:  GET_ARRAY_FROM_MASTER(long long int,MPI_LONG_LONG_INT); break;
		}

		double * tmp_ptr = (double *) ptr;

		cl_buffer_region send_lower_info, send_upper_info;
		cl_buffer_region recv_lower_info, recv_upper_info;
		cl_mem cl_send_lower, cl_send_upper;
		cl_mem cl_recv_lower, cl_recv_upper;
		MPI_Status mpi_status;
		MPI_Request send_lower_req, send_upper_req;
		MPI_Request recv_lower_req, recv_upper_req;

		int lower_bound = pattern_array[0];
		int upper_bound = pattern_array[1];
		int lower_bound_size = abs(lower_bound) * block_size;
		int upper_bound_size = abs(upper_bound) * block_size;

		int type_size = _CalcTypeSize(type_id);
		MPI_Datatype mpi_type = _GetMpiDatatype(type_id);

		// FIXME : remove this constraint
		if (lower_bound_size != upper_bound_size)
			fprintf(stderr, "ERROR %s:%d : in pattern lower and upper bound are not equal,"
				" lower bound size is %d and upper bound size is %d\n",
				__FILE__, __LINE__, lower_bound, upper_bound);

	if (level2_cond) {
		// create send lower bound subbuffer
		if (_gfn_rank != 0 && lower_bound_size > 0) {
			send_lower_info.origin = (size_t)(disp[_gfn_rank]*type_size);
			send_lower_info.size = (size_t)(lower_bound_size*type_size);
			cl_send_lower = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &send_lower_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE SEND LOWER BOUND");
		}
		// create send upper bound subbuffer
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0) {
			send_upper_info.origin = (size_t)((disp[_gfn_rank+1]-upper_bound_size)*type_size);
			send_upper_info.size = (size_t)(upper_bound_size*type_size);
			cl_send_upper = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &send_upper_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE SEND UPPER BOUND");
		}
	
		// download send lower bound subbuffer from GPU to host
		if (_gfn_rank != 0 && lower_bound_size > 0) {
			_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, cl_send_lower, CL_FALSE,
				0, lower_bound_size*type_size, tmp_ptr+disp[_gfn_rank],
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "DOWNLOAD SEND LOWER BOUND FROM DEVICE");
			
		}
		// download send upper bound subbuffer from GPU to host
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0) {
			_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, cl_send_upper, CL_FALSE,
				0, upper_bound_size*type_size, tmp_ptr+disp[_gfn_rank+1]-upper_bound_size,
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "DOWNLOAD SEND UPPER BOUND FROM DEVICE");
		}
		_gfn_status = clFinish(_gfn_cmd_queue);
	}

				// send lower bound asynchonously
		if (_gfn_rank != 0 && lower_bound_size > 0) {
			MPI_Isend((void*)(tmp_ptr+disp[_gfn_rank]), lower_bound_size, 
				mpi_type, _gfn_rank-1, SEND_LOWER_BOUND_TAG, MPI_COMM_WORLD, &send_lower_req);
		}
		// send upper bound asynchonously
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0) {
			MPI_Isend((void*)(tmp_ptr+disp[_gfn_rank+1]-upper_bound_size), upper_bound_size, 
				mpi_type, _gfn_rank+1, SEND_UPPER_BOUMD_TAG, MPI_COMM_WORLD, &send_upper_req);
		}
		// recieve lower bound asynchonously
		if (_gfn_rank != 0 && upper_bound_size > 0) {
			MPI_Irecv((void*)(tmp_ptr+disp[_gfn_rank]-upper_bound_size), upper_bound_size,
				mpi_type, _gfn_rank-1, SEND_UPPER_BOUMD_TAG, MPI_COMM_WORLD, &recv_lower_req);
		}
		// recieve upper bound asynchonously
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0) {
			MPI_Irecv((void*)(tmp_ptr+disp[_gfn_rank+1]), lower_bound_size, 
				mpi_type, _gfn_rank+1, SEND_LOWER_BOUND_TAG, MPI_COMM_WORLD, &recv_upper_req);
		}


		// synchonize recieve
		if (_gfn_rank != (_gfn_num_proc-1) && upper_bound_size > 0)
			MPI_Wait(&send_upper_req, &mpi_status);
		if (_gfn_rank != 0 && lower_bound_size > 0)
			MPI_Wait(&send_lower_req, &mpi_status);
		if (_gfn_rank != 0 && upper_bound_size > 0)
			MPI_Wait(&recv_lower_req, &mpi_status);
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0)
			MPI_Wait(&recv_upper_req, &mpi_status);


	if (level2_cond) {
		// create recieve lower bound subbuffer
		if (_gfn_rank != 0 && upper_bound_size > 0) {
			recv_lower_info.origin = (size_t)((disp[_gfn_rank]-upper_bound_size)*type_size);
			recv_lower_info.size = (size_t)(upper_bound_size*type_size);
			cl_recv_lower = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &recv_lower_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE RECIEVE LOWER BOUND");
		}
		// create recieve upper bound subbuffer
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0) {
			recv_upper_info.origin = (size_t)(disp[_gfn_rank+1]*type_size);
			recv_upper_info.size = (size_t)(lower_bound_size*type_size);
			cl_recv_upper = clCreateSubBuffer(cl_ptr, CL_MEM_READ_WRITE,
				CL_BUFFER_CREATE_TYPE_REGION, &recv_upper_info, &_gfn_status);
			_GfnCheckCLStatus(_gfn_status, "CREATE RECIEVE UPPER BOUND");
		}

		// upload upper bound to device
		if (_gfn_rank != 0 && upper_bound_size > 0) {
			_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_recv_lower, CL_FALSE, 
				0, upper_bound_size*sizeof(double), (tmp_ptr+disp[_gfn_rank]-upper_bound_size),
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "UPLOAD RECIEVE UPPER BOUND FROM HOST");
		}
		// upload lower bound to device
		if (_gfn_rank != (_gfn_num_proc-1) && lower_bound_size > 0) {
			_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, cl_recv_upper, CL_FALSE, 
				0, lower_bound_size*sizeof(double), (tmp_ptr+disp[_gfn_rank+1]),
				0, NULL, NULL);
			_GfnCheckCLStatus(_gfn_status, "UPLOAD RECIEVE LOWER BOUND FROM HOST");
		}
		_gfn_status = clFinish(_gfn_cmd_queue);
	}

	}

	#define SWITCH_STREAMSEQ_SCATTER_ND(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	if (_gfn_rank == 0) { \
		_RecvInputNDMsgCore(tmp_ptr,type_id,loop_start,loop_end,loop_step, \
						partitioned_dim,pattern_type,size_n,pattern_n,size_array,pattern_array); \
	} \
for (i = 0; i < recv_loop_num; ++i) { \
	MPI_Scatterv(tmp_ptr + recv_it_offset, cnts, disp, mpi_type, \
		tmp_ptr + recv_it_offset + recv_elem_offset, sub_size, mpi_type, 0, MPI_COMM_WORLD); \
	if (level2_cond) { \
		info.origin = (size_t)(recv_elem_offset * sizeof(type)); \
		info.size = (size_t)(sub_size * sizeof(type)); \
		subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
		_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + recv_it_offset + recv_elem_offset, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
		_gfn_status = clReleaseMemObject(subbuf); \
		_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
	} \
	recv_it_offset += (elem_num * block_size); \
} \
} while (0)

	switch(type_id)
	{
	case TYPE_CHAR:           SWITCH_STREAMSEQ_SCATTER_ND(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAMSEQ_SCATTER_ND(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAMSEQ_SCATTER_ND(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAMSEQ_SCATTER_ND(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAMSEQ_SCATTER_ND(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAMSEQ_SCATTER_ND(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAMSEQ_SCATTER_ND(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAMSEQ_SCATTER_ND(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAMSEQ_SCATTER_ND(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAMSEQ_SCATTER_ND(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAMSEQ_SCATTER_ND(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAMSEQ_SCATTER_ND(long long int,MPI_LONG_LONG_INT); break;
	}
#endif

	return 0;
}

int _GfnStreamSeqIScatter(struct _data_information *data_info)
{	
	int *cnts = data_info->last_partition_cnts;
	int *disp = data_info->last_partition_disp;
	int sub_size = data_info->last_partition_cnts[_gfn_rank];
	int elem_offset = data_info->last_partition_disp[_gfn_rank];
	
	if (sub_size == -1) {
		data_info->has_iscatter_req = 0; /* FALSE */
		return 0;
	}
	
	data_info->has_iscatter_req = 1; /* TRUE */
	
#define SWITCH_STREAM_SCATTER(type, mpi_type) \
do { \
	type * tmp_ptr = (type *) data_info->ptr; \
	if (_gfn_rank == 0) \
		MPI_Iscatterv(tmp_ptr, cnts, disp, mpi_type, \
			MPI_IN_PLACE, sub_size, mpi_type, 0, MPI_COMM_WORLD, \
			&(data_info->last_iscatter_req)); \
	else \
		MPI_Iscatterv(tmp_ptr, cnts, disp, mpi_type, \
			(tmp_ptr + elem_offset), sub_size, mpi_type, 0, MPI_COMM_WORLD, \
			&(data_info->last_iscatter_req)); \
} while (0)

	switch(data_info->type_id)
	{
	case TYPE_CHAR:           SWITCH_STREAM_SCATTER(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAM_SCATTER(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAM_SCATTER(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAM_SCATTER(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAM_SCATTER(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAM_SCATTER(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAM_SCATTER(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAM_SCATTER(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAM_SCATTER(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAM_SCATTER(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAM_SCATTER(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAM_SCATTER(long long int,MPI_LONG_LONG_INT); break;
	}
}

// write sub buffer to accelerator
int _GfnStreamSeqWriteBuffer(struct _data_information *data_info)
{
	cl_buffer_region info;
	cl_mem subbuf;
	
	int sub_size = data_info->last_upload_cnts[_gfn_rank];
	int elem_offset = data_info->last_upload_disp[_gfn_rank];
	
	if (sub_size == -1) {
		data_info->has_upload_evt = 0; /* FALSE */
		return 0;
	}
	
	data_info->has_upload_evt = 1; /* TRUE */
	
#define SWITCH_STREAM_WRITE_BUFFER(type, mpi_type) \
do { \
	type * tmp_ptr = (type *) data_info->ptr; \
	info.origin = (size_t)(elem_offset * sizeof(type)); \
	info.size = (size_t)(sub_size * sizeof(type)); \
	subbuf = clCreateSubBuffer(data_info->cl_ptr, data_info->mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
	_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
	_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, subbuf, CL_FALSE, 0, sizeof(type) * sub_size, tmp_ptr + elem_offset, 0, 0, &(data_info->last_upload_evt)); \
	_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
	_gfn_status = clReleaseMemObject(subbuf); \
	_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
} while (0)
	
#if 0
#define SWITCH_STREAMSEQ_SCATTER_ND(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) data_info->ptr; \
	for (i = 0; i < recv_loop_num; ++i) { \
		info.origin = (size_t)(recv_elem_offset * sizeof(type)); \
		info.size = (size_t)(sub_size * sizeof(type)); \
		subbuf = clCreateSubBuffer(cl_ptr, mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
		_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
		_gfn_status = clEnqueueWriteBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + recv_it_offset + recv_elem_offset, 0, 0, 0); \
		_GfnCheckCLStatus(_gfn_status, "WRITE BUFFER"); \
		_gfn_status = clReleaseMemObject(subbuf); \
		_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
		recv_it_offset += (elem_num * block_size); \
	} \
} while (0)
#endif

	switch(data_info->type_id)
	{
	case TYPE_CHAR:           SWITCH_STREAM_WRITE_BUFFER(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAM_WRITE_BUFFER(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAM_WRITE_BUFFER(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAM_WRITE_BUFFER(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAM_WRITE_BUFFER(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAM_WRITE_BUFFER(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAM_WRITE_BUFFER(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAM_WRITE_BUFFER(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAM_WRITE_BUFFER(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAM_WRITE_BUFFER(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAM_WRITE_BUFFER(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAM_WRITE_BUFFER(long long int,MPI_LONG_LONG_INT); break;
	}
}

int _GfnStreamSeqFinishDistributeArray()
{
	return 0;
}

int _GfnStreamSeqEnqueueGatherND(struct _kernel_information *ker_info, void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ... )
{
	if (_is_lock_transfer(unique_id)) {
		//send_only_bound = 1;
		if (pattern_n == 0)
			return 0;
	}

	int sub_size;
	int send_elem_offset = 0;
	int send_it_offset = 0;		/* increase for (+=) (elem_num * block_size) */
	int send_loop_num = 1;
	int size_array[size_n], pattern_array[pattern_n];
	struct _data_information *data_info = (struct _data_information *) malloc(sizeof(struct _data_information));
	_init_data_info(data_info);
	
	int i, found = 0;
	va_list vl;
	
	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	// Calculate block size and elements number
	data_info->elem_num = 1;
	data_info->block_size = 1;
	for (i = 0; i < size_n; ++i) {
		if (i < partitioned_dim)         send_loop_num *= size_array[i];
		else if (i == partitioned_dim)   data_info->elem_num      *= size_array[i];
		else /* i > partitioned_dim */   data_info->block_size    *= size_array[i];
	}
	
	if (_debug_stream_seq && _gfn_rank == 0) {
		printf("ELEMENT NUM = %zu\n", data_info->elem_num);
		printf("BLOCK SIZE = %zu\n", data_info->block_size);
	}

	// FIXME: this quick fix for partition of loop and data isnot match
	if (partitioned_dim >= 0) {
		loop_start = 0;
		loop_end = size_array[partitioned_dim] - 1;
		loop_step = 1;
	}
	_set_data_info_loop(data_info, loop_start, loop_end, loop_step);

	// Not have information case
	/*if (size_n == 0) {
		// TODO: get other information from var table
		_get_var_info(unique_id, &mem_type, &found);
		if (!found)
			fprintf(stdout, "ERROR %s:%d : cannot get variable info in _GfnEnqueueScatterND", __FILE__, __LINE__);
	}*/
	
	_set_data_info_variable(data_info, unique_id, ptr, cl_ptr, mem_type, type_id);

	_add_gather_var_id( ker_info, data_info ); // Register to kernel
	
	// Calculate disp (cnts is depend on partition size)
	_CalcPartitionInfo(
		data_info->elem_num, 
		data_info->block_size, 
		data_info->loop_start,
		data_info->loop_end, 
		data_info->loop_step, 
		data_info->pattern_array, 
		data_info->pattern_num, 
		data_info->pattern_type, 
		data_info->last_partition_cnts, /* OUTPUT */
		data_info->last_partition_disp  /* OUTPUT */
	);
	for (i = 0; i < _gfn_num_proc; ++i)
		data_info->end_disp[i] = data_info->last_partition_disp[i] + data_info->last_partition_cnts[i];

	return 0;
}


int _GfnStreamSeqFinishGatherND(struct _kernel_information *ker_info, void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ...)
{
	if (_is_lock_transfer(unique_id)) {
		//send_only_bound = 1;
		if (pattern_n == 0) // no dependency
			return 0;
	}
	
	int size_array[size_n], pattern_array[pattern_n];
	
	int i;
	va_list vl;
	
	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);
	
	switch(type_id)
	{
#define SWITCH_STREAMSEQ_MASTER_RECV(type,mpi_type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	if (_gfn_rank == 0) { \
		_SendOutputNDMsgCore(tmp_ptr,type_id,loop_start,loop_end,loop_step, \
			partitioned_dim,pattern_type,size_n,pattern_n,size_array,pattern_array); \
	} \
} while(0)
	case TYPE_CHAR:           SWITCH_STREAMSEQ_MASTER_RECV(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAMSEQ_MASTER_RECV(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAMSEQ_MASTER_RECV(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAMSEQ_MASTER_RECV(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAMSEQ_MASTER_RECV(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAMSEQ_MASTER_RECV(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAMSEQ_MASTER_RECV(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAMSEQ_MASTER_RECV(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAMSEQ_MASTER_RECV(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAMSEQ_MASTER_RECV(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAMSEQ_MASTER_RECV(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAMSEQ_MASTER_RECV(long long int,MPI_LONG_LONG_INT); break;
	}
}

int _GfnStreamSeqIGather(struct _data_information *data_info)
{
	int *cnts = data_info->last_gather_cnts;
	int *disp = data_info->last_gather_disp;
	int sub_size = data_info->last_gather_cnts[_gfn_rank];
	int elem_offset = data_info->last_gather_disp[_gfn_rank];
	
	if (sub_size == -1) {
		data_info->has_igather_req = 0; /* FALSE */
		return 0;
	}
	
	data_info->has_igather_req = 1; /* TRUE */
	
#define SWITCH_STREAM_GATHER(type, mpi_type) \
do { \
	type * tmp_ptr = (type *) data_info->ptr; \
	if (_gfn_rank == 0) \
		MPI_Igatherv(MPI_IN_PLACE, sub_size, mpi_type, tmp_ptr, \
			cnts, disp, mpi_type, 0, MPI_COMM_WORLD, &(data_info->last_igather_req)); \
	else \
		MPI_Igatherv((tmp_ptr + elem_offset), sub_size, mpi_type, tmp_ptr, \
			cnts, disp, mpi_type, 0, MPI_COMM_WORLD, &(data_info->last_igather_req)); \
} while (0)

	switch(data_info->type_id)
	{
	case TYPE_CHAR:           SWITCH_STREAM_GATHER(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAM_GATHER(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAM_GATHER(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAM_GATHER(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAM_GATHER(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAM_GATHER(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAM_GATHER(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAM_GATHER(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAM_GATHER(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAM_GATHER(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAM_GATHER(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAM_GATHER(long long int,MPI_LONG_LONG_INT); break;
	}
}

int _GfnStreamSeqReadBuffer(struct _data_information *data_info)
{
	cl_buffer_region info;
	cl_mem subbuf;
	
	int sub_size = data_info->last_download_cnts[_gfn_rank];
	int elem_offset = data_info->last_download_disp[_gfn_rank];
	
	if (sub_size == -1) {
		data_info->has_download_evt = 0; /* FALSE */
		return 0;
	}
	
	data_info->has_download_evt = 1; /* TRUE */
	
	// TODO: subbuffer memtype should map to pinned memory ??
/*#define SWITCH_STREAM_READ_BUFFER(type, mpi_type) \
do { \
	type * tmp_ptr = (type *) data_info->ptr; \
	info.origin = (size_t)(elem_offset * sizeof(type)); \
	info.size = (size_t)(sub_size * sizeof(type)); \
	subbuf = clCreateSubBuffer(data_info->cl_ptr, data_info->mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
	_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
	_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, subbuf, CL_FALSE, 0, sizeof(type) * sub_size, tmp_ptr + elem_offset, 0, 0, &(data_info->last_download_evt)); \
	_GfnCheckCLStatus(_gfn_status, "READ BUFFER"); \
	_gfn_status = clReleaseMemObject(subbuf); \
	_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
} while (0)*/

#define SWITCH_STREAM_READ_BUFFER(type, mpi_type) \
do { \
	type * tmp_ptr = (type *) data_info->ptr; \
	info.origin = (size_t)(elem_offset * sizeof(type)); \
	info.size = (size_t)(sub_size * sizeof(type)); \
	subbuf = clCreateSubBuffer(data_info->cl_ptr, data_info->mem_type, CL_BUFFER_CREATE_TYPE_REGION, &info, &_gfn_status); \
	_GfnCheckCLStatus(_gfn_status, "CREATE SUB BUFFER"); \
	_gfn_status = clEnqueueReadBuffer(_gfn_cmd_queue, subbuf, CL_TRUE, 0, sizeof(type) * sub_size, tmp_ptr + elem_offset, 0, 0, 0); \
	_GfnCheckCLStatus(_gfn_status, "READ BUFFER"); \
	_gfn_status = clReleaseMemObject(subbuf); \
	_GfnCheckCLStatus(_gfn_status, "RELEASE SUB BUFFER"); \
} while (0)

	switch(data_info->type_id)
	{
	case TYPE_CHAR:           SWITCH_STREAM_READ_BUFFER(char,MPI_CHAR); break;
	case TYPE_UNSIGNED_CHAR:  SWITCH_STREAM_READ_BUFFER(unsigned char,MPI_UNSIGNED_CHAR); break;
	case TYPE_SHORT:          SWITCH_STREAM_READ_BUFFER(short,MPI_SHORT); break;
	case TYPE_UNSIGNED_SHORT: SWITCH_STREAM_READ_BUFFER(unsigned short,MPI_UNSIGNED_SHORT); break;
	case TYPE_INT:            SWITCH_STREAM_READ_BUFFER(int,MPI_INT); break;
	case TYPE_UNSIGNED:       SWITCH_STREAM_READ_BUFFER(unsigned,MPI_UNSIGNED); break;
	case TYPE_LONG:           SWITCH_STREAM_READ_BUFFER(long,MPI_LONG); break;
	case TYPE_UNSIGNED_LONG:  SWITCH_STREAM_READ_BUFFER(unsigned long,MPI_UNSIGNED_LONG); break;
	case TYPE_FLOAT:          SWITCH_STREAM_READ_BUFFER(float,MPI_FLOAT); break;
	case TYPE_DOUBLE:         SWITCH_STREAM_READ_BUFFER(double,MPI_DOUBLE); break;
	case TYPE_LONG_DOUBLE:    SWITCH_STREAM_READ_BUFFER(long double,MPI_LONG_DOUBLE); break;
	case TYPE_LONG_LONG_INT:  SWITCH_STREAM_READ_BUFFER(long long int,MPI_LONG_LONG_INT); break;
	}
}

int _GfnStreamSeqFinishGatherArray()
{
	return 0;
}

int _GfnStreamSeqExec(int start, int end)
{
	if (end == -1 || (start > end))
		return 0; /* FALSE */
	else
		return 1; /* TRUE */
}


int _GfnLockTransfer(long long id)
{
	return _lock_transfer(id);
}

int _GfnUnlockTransfer(long long id)
{
	return _unlock_transfer(id);
}


// Higher Level function
void _GfnBarrier()
{
	MPI_Barrier(MPI_COMM_WORLD);
}

void _GfnCheckCLStatus(cl_int status, const char *phase_name)
{
	if (status != CL_SUCCESS)
	{
		fprintf(stderr, "**[OpenCL] Rank #%d Runtime error in %s phase, kernel : %s**\n", 
			_gfn_rank, phase_name, current_kernel_name);

		switch(status) {
		// program is not a valid program object
		case CL_INVALID_PROGRAM:
		    fprintf(stderr, "Error : CL_INVALID_PROGRAM\n"); break;
		// there is a failure to build the program executable
		case CL_INVALID_ARG_INDEX:
			fprintf(stderr, "Error : CL_INVALID_ARG_INDEX\n"); break;
		case CL_INVALID_ARG_VALUE:
			fprintf(stderr, "Error : CL_INVALID_ARG_VALUE\n"); break;
		case CL_INVALID_SAMPLER:
			fprintf(stderr, "Error : CL_INVALID_SAMPLER\n"); break;
		case CL_INVALID_ARG_SIZE:
			fprintf(stderr, "Error : CL_INVALID_ARG_SIZE\n"); break;
		case CL_BUILD_PROGRAM_FAILURE:
		    fprintf(stderr, "Error : CL_BUILD_PROGRAM_FAILURE\n"); break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			fprintf(stderr, "Error : CL_INVALID_PROGRAM_EXECUTABLE\n"); break;
		case CL_INVALID_BINARY:
		    fprintf(stderr, "Error : CL_INVALID_BINARY\n"); break;
		case CL_INVALID_BUILD_OPTIONS:
		    fprintf(stderr, "Error : CL_INVALID_BUILD_OPTIONS\n"); break;
		case CL_INVALID_COMMAND_QUEUE:
			fprintf(stderr, "Error : CL_INVALID_COMMAND_QUEUE\n"); break;
		case CL_INVALID_KERNEL:
			fprintf(stderr, "Error : CL_INVALID_KERNEL\n"); break;
		case CL_INVALID_KERNEL_NAME:
		    fprintf(stderr, "Error : CL_INVALID_KERNEL_NAME\n"); break;
		case CL_INVALID_KERNEL_DEFINITION:
		    fprintf(stderr, "Error : CL_INVALID_KERNEL_DEFINITION\n"); break;
		case CL_INVALID_CONTEXT:
			fprintf(stderr, "Error : CL_INVALID_CONTEXT\n"); break;
		case CL_INVALID_KERNEL_ARGS:
			fprintf(stderr, "Error : CL_INVALID_KERNEL_ARGS\n"); break;
		case CL_INVALID_WORK_DIMENSION:
			fprintf(stderr, "Error : CL_INVALID_WORK_DIMENSION\n"); break;
		case CL_INVALID_WORK_GROUP_SIZE:
			fprintf(stderr, "Error : CL_INVALID_WORK_GROUP_SIZE\n"); break;
		case CL_INVALID_WORK_ITEM_SIZE:
			fprintf(stderr, "Error : CL_INVALID_WORK_ITEM_SIZE\n"); break;
		case CL_INVALID_GLOBAL_OFFSET:
			fprintf(stderr, "Error : CL_INVALID_GLOBAL_OFFSET\n"); break;
		case CL_INVALID_MEM_OBJECT:
			fprintf(stderr, "Error : CL_INVALID_MEM_OBJECT\n"); break;
		case CL_INVALID_VALUE:
			fprintf(stderr, "Error : CL_INVALID_VALUE\n"); break;
		case CL_INVALID_DEVICE:
		    fprintf(stderr, "Error : CL_INVALID_DEVICE\n"); break;
		case CL_INVALID_OPERATION:
		    fprintf(stderr, "Error : CL_INVALID_OPERATION\n"); break;
		case CL_OUT_OF_RESOURCES:
			fprintf(stderr, "Error : CL_OUT_OF_RESOURCES\n"); break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			fprintf(stderr, "Error : CL_MEM_OBJECT_ALLOCATION_FAILURE\n"); break;
		case CL_INVALID_EVENT_WAIT_LIST:
			fprintf(stderr, "Error : CL_INVALID_EVENT_WAIT_LIST\n"); break;
		case CL_OUT_OF_HOST_MEMORY:
			fprintf(stderr, "Error : CL_OUT_OF_HOST_MEMORY\n"); break;
		case CL_COMPILER_NOT_AVAILABLE:
		    fprintf(stderr, "Error : CL_COMPILER_NOT_AVAILABLE\n"); break;
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			fprintf(stderr, "Error : CL_MISALIGNED_SUB_BUFFER_OFFSET\n"); break;
		case CL_INVALID_BUFFER_SIZE:
			fprintf(stderr, "Error : CL_INVALID_BUFFER_SIZE\n"); break;
		case CL_INVALID_GLOBAL_WORK_SIZE:
			fprintf(stderr, "Error : CL_INVALID_GLOBAL_WORK_SIZE\n"); break;
		default:
		    fprintf(stderr, "Error : CL unknown error value = %d\n", status); break;
		}
	}
}

void _GfnCheckCLStatus2(cl_int status, const char *phase_name, int arg)
{
	char tmp[strlen(phase_name) + 10];
	sprintf(tmp, "%s %d", phase_name, arg);
	_GfnCheckCLStatus(status, tmp);
}

size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num)
{
	size_t div_group_num = ((work_item_num + work_group_item_num - 1) / work_group_item_num);
	return (work_group_item_num * div_group_num);
}

int _GfnCalcLocalDataStart(int data_start, int data_end)
{
	int range_size = data_end - data_start + 1;
	return data_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank);
}

int _GfnCalcLocalDataEnd(int data_start, int data_end)
{
	int range_size = data_end - data_start + 1;
	return data_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank + 1) - 1;
}

#if 0
int _GfnCalcLocalLoopStart(int local_data_start, int loop_start, int loop_step)
{
	// local_start_step = ceil| local_data_start - loop_start |
	//                        | _____________________________ |
	//                        |            loop_step          |
	int local_start_step = ceil((float)(local_data_start - loop_start) / (float)loop_step);
	if (local_start_step < 0) local_start_step = 0;

	return (loop_step * local_start_step) + loop_start;
}

int _GfnCalcLocalLoopEnd(int local_data_end, int loop_end)
{
	return MIN(local_data_end, loop_end);
}
#endif

int _GfnCalcLocalLoopStart(int loop_start, int loop_end, int loop_step)
{
	return _GfnCalcLocalLoopStartCore(loop_start, loop_end, loop_step,
		_gfn_num_proc, _gfn_rank);
}

int _GfnCalcLocalLoopEnd(int loop_start, int loop_end, int loop_step)
{
	return _GfnCalcLocalLoopEndCore(loop_start, loop_end, loop_step,
		_gfn_num_proc, _gfn_rank);
}

int _GfnCalcLocalLoopStartCore(int loop_start, int loop_end, int loop_step,
	int num_proc, int rank)
{
	int range_size = (loop_end - loop_start + 1) / loop_step;
	int start = _CalcOffset(range_size, num_proc, rank) * loop_step;
	return loop_start + start;
}

int _GfnCalcLocalLoopEndCore(int loop_start, int loop_end, int loop_step,
	int num_proc, int rank)
{
	int range_size = (loop_end - loop_start + 1) / loop_step;
	int next_start = _CalcOffset(range_size, num_proc, rank+1) * loop_step;
	return loop_start + next_start - 1;
}

int _CalcLoopSize(int start, int end, int incre)
{
	return ceil((float)(end - start + 1) / (float)incre);
}

int _CalcSubSize(int size, int num_proc, int rank, int block_size)
{
	int result = 0;
	if (rank < 0) return 0; // for start index of rank 0
	result = (size/num_proc) + (int)(rank < (size%num_proc));
	result *= block_size;
	return result;
}

int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank)
{
	int range_size = old_end - old_start + 1;
	return old_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank);
}

int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank)
{
	int range_size = old_end - old_start + 1;
	return old_start + _CalcOffset(range_size, _gfn_num_proc, _gfn_rank + 1);
}

int _CalcOffset(int size, int num_proc, int rank)
{
	int offset = (size/num_proc)*(rank);
	if (rank <= size%num_proc)
		offset += rank;
	else
		offset += size%num_proc;
	return offset;
}

void _CalcCnts(int size, int num_proc, int *cnts /* OUTS */, int block_size)
{
	int i;
	for (i = 0; i < num_proc; ++i) {
		cnts[i] = _CalcSubSize(size, num_proc, i, 1) * block_size;
#ifdef API_DEBUG
		printf("Rank #%d , count[%d] : %d\n", _gfn_rank, i, cnts[i]);
#endif // API_DEBUG
	}
}

void _CalcDisp(int size, int num_proc, int *disp /* OUTS */, int block_size)
{
	int i;
	for (i = 0; i < num_proc; ++i) {
		disp[i] = _CalcOffset(size, num_proc, i) * block_size;
#ifdef API_DEBUG
		printf("Rank #%d , disp[%d] : %d\n", _gfn_rank, i, disp[i]);
#endif // API_DEBUG
	}
}

void _CalcCnts2(int size, int num_proc, int *cnts, int block_size, int offset)
{
	int i;
	for (i = 0; i < num_proc; ++i)
		cnts[i] = (_CalcSubSize(size, num_proc, i, 1) * block_size) + offset;
}

void _CalcDisp2(int size, int num_proc, int *disp, int block_size, int offset)
{
	int i;
	for (i = 0; i < num_proc; ++i)
		disp[i] = (_CalcOffset(size, num_proc, i) * block_size) + offset;
}

size_t _CalcTypeSize(int type_id)
{
	int tid = (int) type_id;

	switch(tid)
	{
		case TYPE_CHAR:           return sizeof(char);
		case TYPE_UNSIGNED_CHAR:  return sizeof(unsigned char);
		case TYPE_SHORT:          return sizeof(short);
		case TYPE_UNSIGNED_SHORT: return sizeof(unsigned short);
		case TYPE_INT:            return sizeof(int);
		case TYPE_UNSIGNED:       return sizeof(unsigned);
		case TYPE_LONG:           return sizeof(long);
		case TYPE_UNSIGNED_LONG:  return sizeof(unsigned long);
		case TYPE_FLOAT:          return sizeof(float);
		case TYPE_DOUBLE:         return sizeof(double);
		case TYPE_LONG_DOUBLE:    return sizeof(long double);
		case TYPE_LONG_LONG_INT:  return sizeof(long long int);
	}
}

void _CalcPartitionInfo(int size, int block_size, int loop_start, int loop_end, int loop_step,
						int *pattern_array, int pattern_array_size, int pattern_type,
	                    int *cnts /* OUTS */, int *disp /* OUTS */)
{
	int i;
	int lower_bound;
	int upper_bound;

	// FIXME: remove pattern array
	pattern_type = PATTERN_NONE;
	pattern_array_size = 0;

	_CalcCnts(size, _gfn_num_proc, cnts, 1);
	_CalcDisp(size, _gfn_num_proc, disp, 1);

	/* Constraint
		1. disp[i] >= loop_start
		2. disp[i] + cnts[i] <= loop_end
	*/
	for (i = 0; i < _gfn_num_proc; ++i) {
		if (disp[i] < loop_start) {
			int distance = loop_start - disp[i];
			disp[i] = loop_start;
			cnts[i] -= distance;
			if (cnts[i] < 0) cnts[i] = 0;
		}
		if (disp[i] + cnts[i] > loop_end) {
			int newsize = loop_end - disp[i] + 1;
			cnts[i] = newsize;
			if (cnts[i] < 0) cnts[i] = 0;
		}
	}

    switch(pattern_type)
    {
    case PATTERN_NONE: break;
    case PATTERN_RANGE:
    	lower_bound = pattern_array[0];
    	upper_bound = pattern_array[1];
#if 0
    	if (_gfn_rank == 0) {
    		printf("pattern lower bound = %d\n", lower_bound);
    		printf("pattern upper bound = %d\n", upper_bound);
    		printf("pattern block size = %d\n", block_size);
    	}
#endif
    	for (i = 0; i < _gfn_num_proc; ++i) {
    		// lower bound is negative TODO: for(i = 100; i > 0; i--) ?
    		if ((disp[i] + (lower_bound/* * block_size*/)) >= 0) {
    			cnts[i] -= (lower_bound/* * block_size*/);
    			disp[i] += (lower_bound/* * block_size*/);
    		}
    		if ((disp[i] + cnts[i] + (upper_bound/* * block_size*/)) <= size) {
    			cnts[i] += (upper_bound/* * block_size*/);
    		}
    	}

    	break;
    case PATTERN_SPECIFIC: /* TODO */ break;
    }

    // multiply all disp and cnts with block size
    for (i = 0; i < _gfn_num_proc; ++i) {
    	disp[i] *= block_size;
    	cnts[i] *= block_size;
	}

#if 0
	if (_gfn_rank == 0) {
    	printf("\n");
    	for (i = 0; i < _gfn_num_proc; ++i) {
    		printf(" - disp[%d] = %d\n", i, disp[i]);
    		printf(" - cnts[%d] = %d\n", i, cnts[i]);
    	}
    	printf("\n");
	}
#endif
}


void _GfnMasterInit()
{
	static int already_init = 0;

	if (already_init)
		return;

	//printf("Initialize master process\n");

	already_init = 1;

	_OpenMasterMsgQueue();

	if (atexit(_GfnMasterFinish)) {
		fprintf(stderr, "Cannot register _GfnMasterFinish as exit function\n");
		exit(1);
	}
}
void _GfnMasterFinish()
{
	//printf("Finish master process\n");

	_SendCallFuncMsg(0);
	_CloseMasterMsgQueue();
}

void _SyncWorker()
{
	int dummy;
	_RecvOutputMsg((void *) &dummy, sizeof(int));
}

void _SyncMaster()
{
	int dummy;
	_SendOutputMsg((void *) &dummy, sizeof(int));
}

int _GFN_TYPE_CHAR()           { return TYPE_CHAR; }
int _GFN_TYPE_UNSIGNED_CHAR()  { return TYPE_UNSIGNED_CHAR; }
int _GFN_TYPE_SHORT()          { return TYPE_SHORT; }
int _GFN_TYPE_UNSIGNED_SHORT() { return TYPE_UNSIGNED_SHORT; }
int _GFN_TYPE_INT()            { return TYPE_INT; }
int _GFN_TYPE_UNSIGNED()       { return TYPE_UNSIGNED; }
int _GFN_TYPE_LONG()           { return TYPE_LONG; }
int _GFN_TYPE_UNSIGNED_LONG()  { return TYPE_UNSIGNED_LONG; }
int _GFN_TYPE_FLOAT()          { return TYPE_FLOAT; }
int _GFN_TYPE_DOUBLE()         { return TYPE_DOUBLE; }
int _GFN_TYPE_LONG_DOUBLE()    { return TYPE_LONG_DOUBLE; }
int _GFN_TYPE_LONG_LONG_INT()  { return TYPE_LONG_LONG_INT; }

MPI_Op _GFN_OP_MAX()              { return MPI_MAX; }
MPI_Op _GFN_OP_MIN()              { return MPI_MIN; }
MPI_Op _GFN_OP_SUM()              { return MPI_SUM; }
MPI_Op _GFN_OP_PROD()             { return MPI_PROD; }
MPI_Op _GFN_OP_LAND()             { return MPI_LAND; }
MPI_Op _GFN_OP_BAND()             { return MPI_BAND; }
MPI_Op _GFN_OP_LOR()              { return MPI_LOR; }
MPI_Op _GFN_OP_BOR()              { return MPI_BOR; }
MPI_Op _GFN_OP_LXOR()             { return MPI_LXOR; }
MPI_Op _GFN_OP_BXOR()             { return MPI_BXOR; }
MPI_Op _GFN_OP_MINLOC()           { return MPI_MINLOC; }
MPI_Op _GFN_OP_MAXLOC()           { return MPI_MAXLOC; }
MPI_Datatype _GetMpiDatatype(int type_id)
{
	switch (type_id) {
	case TYPE_CHAR:           return MPI_CHAR;
	case TYPE_UNSIGNED_CHAR:  return MPI_UNSIGNED_CHAR;
	case TYPE_SHORT:          return MPI_SHORT;
	case TYPE_UNSIGNED_SHORT: return MPI_UNSIGNED_SHORT;
	case TYPE_INT:            return MPI_INT;
	case TYPE_UNSIGNED:       return MPI_UNSIGNED;
	case TYPE_LONG:           return MPI_LONG;
	case TYPE_UNSIGNED_LONG:  return MPI_UNSIGNED_LONG;
	case TYPE_FLOAT:          return MPI_FLOAT;
	case TYPE_DOUBLE:         return MPI_DOUBLE;
	case TYPE_LONG_DOUBLE:    return MPI_LONG_DOUBLE;
	case TYPE_LONG_LONG_INT:  return MPI_LONG_LONG_INT;
	default:
		fprintf(stderr, "Unknown type in _GetMpiDatatype\n");
		return 0;
	}
}

cl_bool _GFN_TRUE()                     { return CL_TRUE; }
cl_bool _GFN_FALSE()                    { return CL_FALSE; }

cl_mem_flags _GFN_MEM_READ_WRITE()      { return CL_MEM_READ_WRITE; }
cl_mem_flags _GFN_MEM_WRITE_ONLY()      { return CL_MEM_WRITE_ONLY; }
cl_mem_flags _GFN_MEM_READ_ONLY()       { return CL_MEM_READ_ONLY; }
cl_mem_flags _GFN_MEM_USE_HOST_PTR()    { return CL_MEM_USE_HOST_PTR; }
cl_mem_flags _GFN_MEM_ALLOC_HOST_PTR()  { return CL_MEM_ALLOC_HOST_PTR; }


// Function for OpenCL
void _InitOpenCL()
{
	cl_uint /*num_platforms, */num_devices;
	
	_gfn_status = clGetPlatformIDs(1, &_gfn_platform_id, NULL/*&num_platforms*/);
	_GfnCheckCLStatus(_gfn_status, "clGetPlatformIDs");

	_gfn_status = clGetDeviceIDs(_gfn_platform_id, CL_DEVICE_TYPE_DEFAULT/*CL_DEVICE_TYPE_GPU*/, 1,
		&_gfn_device_id, &num_devices);
	_GfnCheckCLStatus(_gfn_status, "clGetDeviceIDs");

	_gfn_context = clCreateContext(NULL, 1, &_gfn_device_id,
		NULL, NULL, &_gfn_status);
	_GfnCheckCLStatus(_gfn_status, "clCreateContext");

	_gfn_cmd_queue = clCreateCommandQueue(_gfn_context, _gfn_device_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &_gfn_status);
	_GfnCheckCLStatus(_gfn_status, "clCreateCommandQueue");
}

cl_platform_id _gfn_platform_id;
cl_device_id _gfn_device_id;
cl_context _gfn_context;
cl_command_queue _gfn_cmd_queue;
cl_int _gfn_status;

void _FinalOpenCL()
{
	// TODO: move to before send data back
	// Block until all commands in queue have completed
	//_gfn_status = clFlush(_gfn_cmd_queue);
	//_GfnCheckCLStatus(_gfn_status, "clFlush");

	// Block until all commands in queue have been removed from the queue
    _gfn_status = clFinish(_gfn_cmd_queue);
    _GfnCheckCLStatus(_gfn_status, "FINISH COMMAND QUEUE");

	_gfn_status = clReleaseContext(_gfn_context);
	_GfnCheckCLStatus(_gfn_status, "RELEASE CONTEXT");
}

void _GfnCreateProgram(const char *src)
{
	size_t param_value_size = 1024 * 1024;
    size_t param_value_size_ret;
    char param_value[param_value_size];

    _gfn_cl_program = clCreateProgramWithSource(_gfn_context, 1, 
            (const char **)&src, NULL, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE PROGRAM WITH SOURCE");
    _gfn_status = clBuildProgram(_gfn_cl_program, 1, &_gfn_device_id, NULL, NULL, NULL);
    _GfnCheckCLStatus(_gfn_status, "BUILD PROGRAM");

    /* Get kernel compiler messages */
    _gfn_status = clGetProgramBuildInfo(_gfn_cl_program,
                                   _gfn_device_id,
                                   CL_PROGRAM_BUILD_LOG,
                                   param_value_size,
                                   param_value,
                                   &param_value_size_ret);

    // TODO: why length 2 for success build
    if (param_value_size_ret != 2) {
        printf("Message from kernel compiler : \n%s\n", param_value);
    }

    /* OUTPUT is _gfn_cl_program */
}

void _GfnClearProgram()
{
	_gfn_status = clReleaseProgram(_gfn_cl_program);
	_GfnCheckCLStatus(_gfn_status, "clReleaseProgram");
}

cl_kernel _GfnCreateKernel(const char *name)
{
	// TODO: save kernel, so don't need to recompile
	cl_kernel kernel = NULL;
	
	strcpy(current_kernel_name, name);
	
    kernel = clCreateKernel(_gfn_cl_program, name, &_gfn_status);
    _GfnCheckCLStatus(_gfn_status, "CREATE KERNEL");
    
    return kernel;
}

void _GfnClearKernel(cl_kernel kernel)
{
	_gfn_status = clReleaseKernel(kernel);
	_GfnCheckCLStatus(_gfn_status, "RELEASE KERNEL");
	
	//printf("kernel %s complete\n", current_kernel_name);
}

void _GfnSetKernelArg(cl_kernel kernel, int arg_num, size_t size, void *ptr)
{
	_gfn_status = clSetKernelArg(kernel, arg_num, size, ptr);
	_GfnCheckCLStatus2(_gfn_status, "SET KERNEL ARG", arg_num);
}

void _GfnLaunchKernel(cl_kernel kernel, const size_t *global_size, const size_t *local_size, struct _kernel_information *ker_info)
{
	long long run_kernel_start_t, run_kernel_end_t;

	run_kernel_start_t = get_time();			/* start time */

	if (ker_info)
	{
		_gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue,   /* command queue */
							kernel,        /* kernel */
							1,             /* work dimension */
							0,             /* global work offset */
							global_size,   /* global work size */
							local_size,    /* local work size */
							0,             /* number of event in wait list */
							0,             /* event wait list */
							&(ker_info->exec_evt));  /* event */
		ker_info->has_exec_evt = 1;
	}
	else
	{
		_gfn_status = clEnqueueNDRangeKernel(_gfn_cmd_queue,   /* command queue */
							kernel,        /* kernel */
							1,             /* work dimension */
							0,             /* global work offset */
							global_size,   /* global work size */
							local_size,    /* local work size */
							0,             /* number of event in wait list */
							0,             /* event wait list */
							0);            /* event */
	}
	_GfnCheckCLStatus(_gfn_status, "LAUNCH KERNEL");

	_gfn_status = clFinish(_gfn_cmd_queue);
	_GfnCheckCLStatus(_gfn_status, "FINISH KERNEL");

	run_kernel_end_t = get_time();				/* end time */
	_gfn_last_kernel_time = run_kernel_end_t - run_kernel_start_t;

	IF_TIMING (_gpu_kernel_time) {
		printf("[%d] Launch kernel : %2.6f s. (global size %zu , local size %zu)\n", _gfn_rank, 
			(double)_gfn_last_kernel_time/1000000.0, *global_size, *local_size);
	}

}




//// Function for data information
void _init_data_info(struct _data_information *data_info)
{
	data_info->ptr = NULL;
	data_info->dim_n = 0;
	
	data_info->has_upload_evt = 0;
	data_info->has_download_evt = 0;
	data_info->has_iscatter_req = 0;
	data_info->has_igather_req = 0;
}

void _set_data_info_loop(struct _data_information *data_info, 
			int loop_start, int loop_end, int loop_step)
{
	data_info->loop_start = loop_start;
	data_info->loop_end = loop_end;
	data_info->loop_step = loop_step;
}

void _set_data_info_pattern(struct _data_information *data_info,
			int pattern_type, int pattern_num, int *pattern_array)
{
	int i;
	
	data_info->pattern_type = pattern_type;
	data_info->pattern_num = pattern_num;
	for (i = 0; i < pattern_num; ++i)
		data_info->pattern_array[i] = pattern_array[i];
}

void _set_data_info_variable(struct _data_information *data_info,
			long long unique_id, void *ptr, cl_mem cl_ptr,
			cl_map_flags mem_type, int type_id)
{
	data_info->unique_id = unique_id;
	data_info->ptr = ptr;
	data_info->cl_ptr = cl_ptr;
	data_info->mem_type = mem_type;
	data_info->type_id = type_id;
}


void _update_kernel_info_seq(struct _kernel_information *ker_info)
{
	ker_info->last_gather_seq_start = ker_info->last_download_seq_start;
	ker_info->last_gather_seq_end = ker_info->last_download_seq_end;
	ker_info->last_gather_partition_size = ker_info->last_download_partition_size;
	
	ker_info->last_download_seq_start = ker_info->save_exec_seq_start;
	ker_info->last_download_seq_end = ker_info->save_exec_seq_end;
	ker_info->last_download_partition_size = ker_info->last_exec_partition_size;
	
	ker_info->save_exec_seq_start = ker_info->last_upload_seq_start;
	ker_info->save_exec_seq_end = ker_info->last_upload_seq_end;
	ker_info->last_exec_partition_size = ker_info->last_upload_partition_size;
	
	ker_info->last_upload_seq_start = ker_info->last_partition_seq_start;
	ker_info->last_upload_seq_end = ker_info->last_partition_seq_end;
	ker_info->last_upload_partition_size = ker_info->last_partition_partition_size;
}

void _add_scatter_var_id( struct _kernel_information *ker_info, struct _data_information *var_data ) {
	size_t cur_var_num = ker_info->scatter_var_num;
	ker_info->scatter_var_datas[cur_var_num] = var_data;
	ker_info->scatter_var_num += 1;
}

void _get_scatter_var_ids( struct _kernel_information *ker_info, struct _data_information ***var_datas, size_t *var_num ) {
	*var_num = ker_info->scatter_var_num;
	*var_datas = ker_info->scatter_var_datas;
}

void _add_gather_var_id( struct _kernel_information *ker_info, struct _data_information *var_data ) {
	size_t cur_var_num = ker_info->gather_var_num;
	ker_info->gather_var_datas[cur_var_num] = var_data;
	ker_info->gather_var_num += 1;
}

void _get_gather_var_ids( struct _kernel_information *ker_info, struct _data_information ***var_datas, size_t *var_num ) {
	*var_num = ker_info->gather_var_num;
	*var_datas = ker_info->gather_var_datas;
}
