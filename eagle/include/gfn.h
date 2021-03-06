#ifndef GFN_H
#define GFN_H

#include <stdarg.h>
#include <mqueue.h>
#include <ibcast.h>
#include <sys/stat.h>
#include <mpi.h>
#include <CL/cl.h>


/* Debugging */
//#define DEBUG_AUTO_ALLOCATE
//#define DEBUG_VAR_TABLE
//#define DEBUG_BOUND
//#define DEBUG_INTER_PROC_COMM

/* Optimization */
#define OPTIMIZE_NO_USE_CL_SUBBUFFER
#define OPTIMIZE_STREAM_SEQ

/* Constant */
#define MAX_DIM_SUPPORT 6

#define MAX_IBCAST_HANDLE_LIST 20

extern int _gfn_opt_level;
extern int _gfn_rank;		/**/
extern int _gfn_num_proc;	/**/
extern cl_platform_id _gfn_platform_id;
extern cl_device_id _gfn_device_id;
extern cl_context _gfn_context;
extern cl_command_queue _gfn_cmd_queue;
extern cl_int _gfn_status;
extern cl_program _gfn_cl_program;

extern int _debug_stream_seq;

extern long long _gfn_last_kernel_time;

extern char current_kernel_name[50];

extern Ibcast_handle *send_scalar_handles[MAX_IBCAST_HANDLE_LIST];
extern int send_scalar_curr_idx;

struct _data_information {
	long long unique_id;
	void *ptr;
	cl_mem cl_ptr;
	cl_mem_flags mem_type;
	int type_id;
	
	size_t dim_n;
	size_t dim_size[7];
	
	size_t elem_num;
	size_t block_size;
	
	int loop_start;
	int loop_end;
	int loop_step;
	
	int pattern_type;
	int pattern_array[40];
	int pattern_num;
	
	// For gather
	int last_gather_cnts[8];
	int last_gather_disp[8];
	
	// For read buffer
	int last_download_cnts[8];
	int last_download_disp[8];
	
	// For kernel execution
	int last_exec_cnts[8];
	int last_exec_disp[8];
	
	// For write buffer
	int last_upload_cnts[8];
	int last_upload_disp[8];
	
	// For broadcast and scatter
	int last_partition_cnts[8];
	int last_partition_disp[8];
	int end_disp[8];
	
	int has_upload_evt;
	cl_event last_upload_evt;
	int has_download_evt;
	cl_event last_download_evt;
	int has_iscatter_req;
	MPI_Request last_iscatter_req;
	int has_igather_req;
	MPI_Request last_igather_req;
};

void _set_data_info_variable(struct _data_information *data_info,
			long long unique_id, void *ptr, cl_mem cl_ptr,
			cl_map_flags mem_type, int type_id);
void _set_data_info_loop(struct _data_information *data_info, 
			int loop_start, int loop_end, int loop_step);
void _set_data_info_pattern(struct _data_information *data_info,
			int pattern_type, int pattern_num, int *pattern_array);

struct _kernel_information {
	
	long long kernel_id;
	int local_start;
	int local_end;
	int loop_start;
	int loop_end;
	int loop_step;
	
	int curr_sequence_id;
	int stream_seq_start_idx;
	int stream_seq_end_idx;
	int is_complete;
	
	// For gather
	int last_gather_seq_start;
	int last_gather_seq_end;
	int last_gather_partition_size;
	
	// For read buffer
	int last_download_seq_start;
	int last_download_seq_end;
	int last_download_partition_size;
	
	// For kernel execution
	int save_exec_seq_start;
	int save_exec_seq_end;
	int last_exec_partition_size;
	
	// For write buffer
	int last_upload_seq_start;
	int last_upload_seq_end;
	int last_upload_partition_size;
	
	// For broadcast and scatter
	int last_partition_seq_start;
	int last_partition_seq_end;
	int last_partition_partition_size;
	
	cl_event exec_evt;
	int has_exec_evt;
	
	// TODO: Broadcast
	size_t bcast_var_num;
	struct _data_information *bcast_var_datas[15]; // TODO: Don't fix array size
	// Scatter 
	size_t scatter_var_num;
	struct _data_information *scatter_var_datas[15]; // TODO: Don't fix array size
	// Gather
	size_t gather_var_num;
	struct _data_information *gather_var_datas[15]; // TODO: Don't fix array size
};

void _update_kernel_info_seq(struct _kernel_information *ker_info);
void _add_scatter_var_id( struct _kernel_information *ker_info, struct _data_information *var_data );
void _get_scatter_var_ids( struct _kernel_information *ker_info, struct _data_information ***var_datas, size_t *var_num );
void _add_gather_var_id( struct _kernel_information *ker_info, struct _data_information *var_data );
void _get_gather_var_ids( struct _kernel_information *ker_info, struct _data_information ***var_datas, size_t *var_num );


// API for user
int gfn_get_num_process();
int gfn_get_process_num();


// Worker
int _GfnInit(int *argc, char **argv[]);
int _GfnFinalize();

int _GfnMallocReduceScalar(void * ptr, cl_mem *cl_ptr, int type_id, int group_num, int level1_cond, int level2_cond);
int _GfnFreeReduceScalar(cl_mem cl_ptr, int level1_cond, int level2_cond);
int _GfnEnqueueBroadcastScalar(void *ptr, int type_id); /*-> recv, bcast*/
int _GfnFinishBroadcastScalar();
int _GfnEnqueueReduceScalar(void *ptr, cl_mem cl_ptr, int type_id, MPI_Op op_id, int group_num, int level1_cond, int level2_cond);
int _GfnFinishReduceScalar();
// TODO: _GfnEnqueueReduce1D() ???
int _GfnMalloc1D(void ** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc2D(void *** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc3D(void **** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc4D(void ***** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc5D(void ****** ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
int _GfnMalloc6D(void ******* ptr, cl_mem *cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int level1_cond, int level2_cond);
// Potential to free, maybe not really free
int _GfnFree(long long unique_id, int level1_cond, int level2_cond);
// TODO: add broadcast pattern as parameter
int _GfnEnqueueBroadcastND(void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, int level1_cond, int level2_cond, int dim_n, ...);
int _GfnEnqueueScatterND(void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ... );
int _GfnFinishDistributeArray();

int _GfnEnqueueGatherND(void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ... );
int _GfnFinishGatherArray();

int _GfnLockTransfer(long long id);
int _GfnUnlockTransfer(long long id);



// Stream Sequence Optimization
int _GfnStreamSeqKernelRegister(long long kernel_id, int local_start, int local_end, int loop_start, int loop_end, int loop_step, struct _kernel_information **out_ker_info);
int _GfnStreamSeqKernelCleanUp(struct _kernel_information *ker_info);
int _GfnStreamSeqKernelGetNextSequence(struct _kernel_information *ker_info, int *seq_id, 
                                       size_t *stream_global_item_num, size_t *stream_work_group_item_num);
int _GfnStreamSeqKernelFinishSequence(struct _kernel_information *ker_info);
int _GfnStreamSeqEnqueueScatterND(struct _kernel_information *ker_info, void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ... );
int _GfnStreamSeqFinishDistributeArray();
int _GfnStreamSeqEnqueueGatherND(struct _kernel_information *ker_info, void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ... );
int _GfnStreamSeqFinishGatherND(struct _kernel_information *ker_info, void * ptr, cl_mem cl_ptr, long long unique_id, int type_id, cl_mem_flags mem_type, 
						int loop_start, int loop_end, int loop_step, int partitioned_dim, int pattern_type, 
						int level1_cond, int level2_cond, int size_n, int pattern_n, ...);
int _GfnStreamSeqFinishGatherArray(); // TODO: clear var_table
int _GfnStreamSeqExec(int start, int end);


// High Level function for trnsformation
void _GfnBarrier();
void _GfnCheckCLStatus(cl_int status, const char *phase_name);
void _GfnCheckCLStatus2(cl_int status, const char *phase_name, int arg);
size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num);
int _GfnCalcLocalDataStart(int data_start, int data_end);    // Use for calculate start step
int _GfnCalcLocalDataEnd(int data_start, int data_end);

int _GfnCalcLocalLoopStart(int loop_start, int loop_end, int loop_step);
int _GfnCalcLocalLoopEnd(int loop_start, int loop_end, int loop_step);
int _GfnCalcLocalLoopStartCore(int loop_start, int loop_end, int loop_step, int num_proc, int rank);
int _GfnCalcLocalLoopEndCore(int loop_start, int loop_end, int loop_step, int num_proc, int rank);

void _GfnMasterInit();
void _GfnMasterFinish();

void _SyncWorker();
void _SyncMaster();

int _CalcLoopSize(int start, int end, int incre);
int _CalcSubSize(int size, int num_proc, int rank, int block_size);
int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcOffset(int size, int num_proc, int rank);
void _CalcCnts(int size, int num_proc, int *cnts /* OUTS */, int block_size);
void _CalcDisp(int size, int num_proc, int *disp /* OUTS */, int block_size);
void _CalcCnts2(int size, int num_proc, int *cnts, int block_size, int offset);
void _CalcDisp2(int size, int num_proc, int *disp, int block_size, int offset);
size_t _CalcTypeSize(int type_id);

void _CalcPartitionInfo(int size, int block_size, int loop_start, int loop_end, int loop_step,
						int *pattern_array, int pattern_array_size, int pattern_type,
                        int *cnts /* OUTS */, int *disp /* OUTS */);

// Function for fix preprocessing of mcxx
int _GFN_TYPE_CHAR();
int _GFN_TYPE_UNSIGNED_CHAR();
int _GFN_TYPE_SHORT();
int _GFN_TYPE_UNSIGEND_SHORT();
int _GFN_TYPE_INT();
int _GFN_TYPE_UNSIGNED();
int _GFN_TYPE_LONG();
int _GFN_TYPE_UNSIGNED_LONG();
int _GFN_TYPE_FLOAT();
int _GFN_TYPE_DOUBLE();
int _GFN_TYPE_LONG_DOUBLE();
int _GFN_TYPE_LONG_LONG_INT();

MPI_Op _GFN_OP_MAX();
MPI_Op _GFN_OP_MIN();
MPI_Op _GFN_OP_SUM();
MPI_Op _GFN_OP_PROD();
MPI_Op _GFN_OP_LAND();
MPI_Op _GFN_OP_BAND();
MPI_Op _GFN_OP_LOR();
MPI_Op _GFN_OP_BOR();
MPI_Op _GFN_OP_LXOR();
MPI_Op _GFN_OP_BXOR();
MPI_Op _GFN_OP_MINLOC();
MPI_Op _GFN_OP_MAXLOC();
MPI_Datatype _GetMpiDatatype(int type_id);

cl_bool _GFN_TRUE();
cl_bool _GFN_FALSE();

cl_mem_flags _GFN_MEM_READ_WRITE();
cl_mem_flags _GFN_MEM_WRITE_ONLY();
cl_mem_flags _GFN_MEM_READ_ONLY();
cl_mem_flags _GFN_MEM_USE_HOST_PTR();
cl_mem_flags _GFN_MEM_ALLOC_HOST_PTR();



// Function for OpenCL
void _InitOpenCL();
void _FinalOpenCL();
void _GfnCreateProgram(const char *src);
void _GfnClearProgram();
cl_kernel _GfnCreateKernel(const char *name);
void _GfnClearKernel(cl_kernel kernel);
void _GfnSetKernelArg(cl_kernel kernel, int arg_num, size_t size, void *ptr);
void _GfnLaunchKernel(cl_kernel kernel, const size_t *global_size, const size_t *local_size, struct _kernel_information *ker_info);

/*----------------------------------------------------------------------------*\
                   IPC INTERFACE - source is in myipc_socket.c
\*----------------------------------------------------------------------------*/

extern mqd_t _master_send_qid;
extern mqd_t _master_recv_qid;
extern mqd_t _worker_send_qid;
extern mqd_t _worker_recv_qid;
//extern int _is_opened;
extern int pid;

void _LaunchWorker();
void _StopWorker();

void _OpenMasterMsgQueue();
void _CloseMasterMsgQueue();
void _OpenWorkerMsgQueue();
void _CloseWorkerMsgQueue();


int _SendConstInputMsg(long long c);

int _SendInputMsg(void *ptr, size_t size);
int _RecvOutputMsg(void *ptr, size_t size);
int _RecvInputMsg(void *ptr, size_t size);
int _SendOutputMsg(void *ptr, size_t size);

int _SendInputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... );
int _SendInputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array );
int _RecvOutputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... );
int _RecvOutputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array );
int _RecvInputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... );
int _RecvInputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array );
int _SendOutputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... );
int _SendOutputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array );

void _SendCallFuncMsg(int func_code);
void _RecvCallFuncMsg(int *func_code);


/* Low level function -- forward declaration */
void _SendMasterMsg(void *buffer, size_t size /*,status&*/);
void _RecvMasterMsg(void *buffer, size_t size /*, status&*/);
void _SendWorkerMsg(void *buffer, size_t size /*,status&*/);
void _RecvWorkerMsg(void *buffer, size_t size /*, status&*/);

#endif
