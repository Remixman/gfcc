#ifndef GFN_H
#define GFN_H

#include <mqueue.h>
#include <sys/stat.h>
#include <mpi.h>
#include <CL/cl.h>


extern int _gfn_rank;		/**/
extern int _gfn_num_proc;	/**/
extern cl_platform_id _gfn_platform_id;
extern cl_device_id _gfn_device_id;
extern cl_context _gfn_context;
extern cl_command_queue _gfn_cmd_queue;
extern cl_int _gfn_status;

// API for user
int gfn_get_num_process();
int gfn_get_process_num();


// Worker
int _GfnInit();
int _GfnFinalize();

//int _GfnEnqueueBoardcastScalar(&n, _get_int_type()); /*-> recv, bcast*/
int _GfnEnqueueBoardcastScalar(void *ptr, int type_id); /*-> recv, bcast*/
int _GfnFinishBoardcastScalar();
int _GfnEnqueueReduceScalar(void *ptr, int type_id, MPI_Op op_id);
int _GfnFinishReduceScalar();
// TODO: _GfnEnqueueReduce1D() ???
    
int _GfnMalloc1D(void ** ptr, cl_mem cl_ptr, long long unique_id, int type_id, size_t dim1_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc);
int _GfnMalloc2D(void *** ptr, cl_mem cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc);
int _GfnMalloc3D(void **** ptr, cl_mem cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc);
int _GfnMalloc4D(void ***** ptr, cl_mem cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc);
int _GfnMalloc5D(void ****** ptr, cl_mem cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc);
int _GfnMalloc6D(void ******* ptr, cl_mem cl_ptr, long long unique_id, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, cl_mem_flags mem_type, int level1_malloc, int level2_malloc);
// TODO: add boardcast pattern as parameter
int _GfnEnqueueBoardcast1D(void ** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, int level1_transfer, int level2_transfer);
int _GfnEnqueueBoardcast2D(void *** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, int level1_transfer, int level2_transfer);
int _GfnEnqueueBoardcast3D(void **** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, int level1_transfer, int level2_transfer);
int _GfnEnqueueBoardcast4D(void ***** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, int level1_transfer, int level2_transfer);
int _GfnEnqueueBoardcast5D(void ****** ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, int level1_transfer, int level2_transfer);
int _GfnEnqueueBoardcast6D(void ******* ptr, cl_mem cl_ptr, int type_id, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, int level1_transfer, int level2_transfer);

int _GfnEnqueueScatter1D(void ** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueScatter2D(void *** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueScatter3D(void **** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueScatter4D(void ***** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueScatter5D(void ****** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueScatter6D(void ******* ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnFinishDistributeArray();

int _GfnEnqueueGather1D(void ** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueGather2D(void *** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueGather3D(void **** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueGather4D(void ***** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueGather5D(void ****** ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnEnqueueGather6D(void ******* ptr, cl_mem cl_ptr, int type_id, int partitioned_dim, size_t dim1_size, size_t dim2_size, size_t dim3_size, size_t dim4_size, size_t dim5_size, size_t dim6_size, int * pattern_array, int pattern_type, int level1_transfer, int level2_transfer);
int _GfnFinishGatherArray();


// High Level function for trnsformation
void _GfnBarrier();
void _GfnCheckCLStatus(cl_int status, const char *phase_name);
size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num);

// Function for MPI
int _CalcLoopSize(int start, int end, int incre);
int _CalcSubSize(int size, int num_proc, int rank, int block_size);
int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcOffset(int size, int num_proc, int rank);
void _CalcCnts(int size, int num_proc, int *cnts /* OUTS */, int block_size);
void _CalcDisp(int size, int num_proc, int *disp /* OUTS */, int block_size);
size_t _CalcTypeSize(int type_id);
void _CalcPartitionInfo(int size, int block_size, int * pattern_array, int pattern_type,
                        int *cnts /* OUTS */, int *disp /* OUTS */, 
                        int *sub_size /* OUTS */, int *elem_offset /* OUTS */);

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
cl_kernel _CreateKernelFromSource(const char *name, const char *src,
                                  cl_context context, cl_device_id device_id);

/*----------------------------------------------------------------------------*\
                   IPC INTERFACE - source is in myipc_socket.c
\*----------------------------------------------------------------------------*/
#define IPC_DEBUG

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

// TODO: if not array remove *size
void _SendInputMsg(void *buffer, size_t size);
void _RecvOutputMsg(void *buffer, size_t size /*, status&*/);
void _RecvInputMsg(void *buffer, size_t size /*, status&*/);
void _SendOutputMsg(void *buffer, size_t size);

void _SendCallFuncMsg(int func_code);
void _RecvCallFuncMsg(int *func_code);


/* Low level function -- forward declaration */
void _SendMasterMsg(void *buffer, size_t size /*,status&*/);
void _RecvMasterMsg(void *buffer, size_t size /*, status&*/);
void _SendWorkerMsg(void *buffer, size_t size /*,status&*/);
void _RecvWorkerMsg(void *buffer, size_t size /*, status&*/);

#endif

