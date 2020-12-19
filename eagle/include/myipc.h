#include <mqueue.h>
#include <sys/stat.h>

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
