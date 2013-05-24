#ifndef GFN_H
#define GFN_H

#include <mqueue.h>
#include <sys/stat.h>
#include <mpi.h>
#include <CL/cl.h>


// API for user
int gfn_get_num_process();
int gfn_get_process_num();


// High Level function for trnsformation
void _GfnBarrier();
void _GfnCheckCLStatus(cl_int status, const char *phase_name);
size_t _GfnCalcGlobalItemNum(size_t work_item_num, size_t work_group_item_num);


// Variable for MPI
extern int _gfn_rank;		/**/
extern int _gfn_num_proc;	/**/

// Function for MPI
int _CalcLoopSize(int start, int end, int incre);
int _CalcSubSize(int size, int num_proc, int rank, int block_size);
int _CalcLocalStartIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcLocalEndIndex(int old_start, int old_end, int num_proc, int rank);
int _CalcOffset(int size, int num_proc, int rank);
void _CalcCnts(int size, int num_proc, int *cnts /* OUTS */, int block_size);
void _CalcDisp(int size, int num_proc, int *disp /* OUTS */, int block_size);

// Function for fix preprocessing of mcxx
MPI_Datatype _get_mpi_char();
MPI_Datatype _get_mpi_unsigned_char();
MPI_Datatype _get_mpi_byte();
MPI_Datatype _get_mpi_short();
MPI_Datatype _get_mpi_unsigned_short();
MPI_Datatype _get_mpi_int();
MPI_Datatype _get_mpi_unsigned();
MPI_Datatype _get_mpi_long();
MPI_Datatype _get_mpi_unsigned_long();
MPI_Datatype _get_mpi_float();
MPI_Datatype _get_mpi_double();
MPI_Datatype _get_mpi_long_double();
MPI_Datatype _get_mpi_long_long_int();

MPI_Op _get_mpi_max();
MPI_Op _get_mpi_min();
MPI_Op _get_mpi_sum();
MPI_Op _get_mpi_prod();
MPI_Op _get_mpi_land();
MPI_Op _get_mpi_band();
MPI_Op _get_mpi_lor();
MPI_Op _get_mpi_bor();
MPI_Op _get_mpi_lxor();
MPI_Op _get_mpi_bxor();
MPI_Op _get_mpi_minloc();
MPI_Op _get_mpi_maxloc();

MPI_Comm _get_mpi_comm_world();

// Variable for OpenCL
extern cl_platform_id _gfn_platform_id;
extern cl_device_id _gfn_device_id;
extern cl_context _gfn_context;
extern cl_command_queue _gfn_cmd_queue;
extern cl_int _gfn_status;

// Function for OpenCL
void _InitOpenCL();
void _FinalOpenCL();
cl_kernel _CreateKernelFromSource(const char *name, const char *src,
                                  cl_context context, cl_device_id device_id);

cl_bool _get_cl_true();
cl_bool _get_cl_false();

cl_mem_flags _get_cl_mem_read_write();
cl_mem_flags _get_cl_mem_write_only();
cl_mem_flags _get_cl_mem_read_only();
cl_mem_flags _get_cl_mem_use_host_ptr();
cl_mem_flags _get_cl_mem_alloc_host_ptr();

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

