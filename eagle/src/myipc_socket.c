#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gfn.h"

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

// http://www.thegeekstuff.com/2011/12/c-socket-programming/

#define MIN(A,B) ((A<B)?(A):(B))
#define MAX(A,B) ((A>B)?(A):(B))

#define USED_PORT 5829
#define MAX_SOCKET_BUFFER_SIZE (4096<<1)
#define MAX_MSG_LEN 1000000000

// For worker server
int listenfd = 0;
int connfd = 0;
struct sockaddr_in serv_addr; // also in master client

// For master client
int sockfd = 0;
struct hostent *hp;

void _LaunchWorker() { }

void _StopWorker() { }

void _OpenMasterMsgQueue() {
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "ERROR: _OpenMasterMsgQueue\n");
		return;
	}
	//memset(recv_buff, 0, sizeof(recv_buff));
	memset((void*)&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(USED_PORT);
	hp = gethostbyname("localhost");

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		fprintf(stderr, "ERROR: _OpenMasterMsgQueue::inet_pton\n");
		return;
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: _OpenMasterMsgQueue::connect\n");
		return;
	}
}

void _CloseMasterMsgQueue() {
	close(sockfd);
}

void _OpenWorkerMsgQueue() {
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset((void*)&serv_addr, 0, sizeof(serv_addr));
	//memset(send_buff, 0, sizeof(send_buff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(USED_PORT);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 1 /* Maximum Client */);

	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
}

void _CloseWorkerMsgQueue() {
	close(connfd);
	close(listenfd);
}

int _SendConstInputMsg(long long c) {
	_SendMasterMsg(&c, sizeof(long long));
	return 0;
}

int _SendInputMsg(	void *ptr, size_t size) {
	_SendMasterMsg(ptr, size);
	return 0;
}

int _RecvOutputMsg(	void *ptr, size_t size) {
	_RecvMasterMsg(ptr, size);
	return 0;
}

int _RecvInputMsg(	void *ptr, size_t size) {
	_RecvWorkerMsg(ptr, size);
	return 0;
}

int _SendOutputMsg(	void *ptr, size_t size) {
	_SendWorkerMsg(ptr, size);
	return 0;
}

static void _CalcStartOffsetAndSize(int *start_offset, int *size,
									int loop_start, int loop_end, int loop_step,
									int partitioned_dim, int pattern_type,
									int size_n, int pattern_n, 
									int *size_array, int *pattern_array) {
	int i, end_offset;
	int nelem_size = 1, block_size = 1;

	// TODO: loop_step != 1 case , partitioned_dim != 1,0 case

	// find element size and block size
	for (i = 0; i < size_n; ++i) {
		if (i == partitioned_dim) 	nelem_size *= size_array[i];
		else  						block_size *= size_array[i];
	}
	
	switch(pattern_type) {
		case PATTERN_NONE:
			(*start_offset) = loop_start;
			end_offset = loop_end;
			break;
		case PATTERN_RANGE:
			(*start_offset) = loop_start + pattern_array[0];
			if ((*start_offset) < 0) (*start_offset) = 0;
			end_offset = loop_end + pattern_array[1];
			if (end_offset > nelem_size) end_offset = nelem_size;
			break;
		case PATTERN_SPECIFIC:
			break;
	}
	(*size) = (end_offset - (*start_offset) + 1) * block_size;
}

int _SendInputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... ) {

	int i, value;
	va_list vl;

	int size_array[size_n], pattern_array[pattern_n];
	partitioned_dim--; // convert partition dimension to zero based

	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	return _SendInputNDMsgCore(ptr, type_id, loop_start, loop_end, loop_step,
				partitioned_dim, pattern_type, size_n, pattern_n, size_array, pattern_array);
}

int _SendInputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array )
{
	int start_offset, send_size;
	int nelem_size = 1, block_size = 1;

	_CalcStartOffsetAndSize(&start_offset, &send_size, loop_start, loop_end, loop_step,
							partitioned_dim, pattern_type,
							size_n, pattern_n, size_array, pattern_array);

#if 0
	printf("START OFFSET = %d\n", start_offset);
	printf("SEND SIZE = %d\n", send_size);
#endif

#define SEND_INPUT_ARRAY(type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	_SendMasterMsg((void*)(tmp_ptr+start_offset), send_size * sizeof(type)); \
} while(0)
	
	//_SendMasterMsg((void*)(((type*)ptr) + start_offset), send_size * sizeof(type));

	switch(type_id)
	{
	case TYPE_CHAR:           SEND_INPUT_ARRAY(char); break;
	case TYPE_UNSIGNED_CHAR:  SEND_INPUT_ARRAY(unsigned char); break;
	case TYPE_SHORT:          SEND_INPUT_ARRAY(short); break;
	case TYPE_UNSIGNED_SHORT: SEND_INPUT_ARRAY(unsigned short); break;
	case TYPE_INT:            SEND_INPUT_ARRAY(int); break;
	case TYPE_UNSIGNED:       SEND_INPUT_ARRAY(unsigned); break;
	case TYPE_LONG:           SEND_INPUT_ARRAY(long); break;
	case TYPE_UNSIGNED_LONG:  SEND_INPUT_ARRAY(unsigned long); break;
	case TYPE_FLOAT:          SEND_INPUT_ARRAY(float); break;
	case TYPE_DOUBLE:         SEND_INPUT_ARRAY(double); break;
	case TYPE_LONG_DOUBLE:    SEND_INPUT_ARRAY(long double); break;
	case TYPE_LONG_LONG_INT:  SEND_INPUT_ARRAY(long long int); break;
	}

	return 0;
}

int _RecvOutputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... ) {

	int i, value;
	va_list vl;

	int size_array[size_n], pattern_array[pattern_n];
	partitioned_dim--; // convert partition dimension to zero based

	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	return _RecvOutputNDMsgCore(ptr, type_id, loop_start, loop_end, loop_step,
				partitioned_dim, pattern_type, size_n, pattern_n, size_array, pattern_array);
}

int _RecvOutputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array )
{
	int start_offset, recv_size;
	int nelem_size = 1, block_size = 1;

	_CalcStartOffsetAndSize(&start_offset, &recv_size, loop_start, loop_end, loop_step,
							partitioned_dim, pattern_type,
							size_n, pattern_n, size_array, pattern_array);	

#define RECV_OUTPUT_ARRAY(type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	_RecvMasterMsg((void*)(tmp_ptr+start_offset), recv_size * sizeof(type)); \
} while(0)
	
	// _RecvMasterMsg((void*)(((type*)ptr) + start_offset), recv_size * sizeof(type));

	switch(type_id)
	{
	case TYPE_CHAR:           RECV_OUTPUT_ARRAY(char); break;
	case TYPE_UNSIGNED_CHAR:  RECV_OUTPUT_ARRAY(unsigned char); break;
	case TYPE_SHORT:          RECV_OUTPUT_ARRAY(short); break;
	case TYPE_UNSIGNED_SHORT: RECV_OUTPUT_ARRAY(unsigned short); break;
	case TYPE_INT:            RECV_OUTPUT_ARRAY(int); break;
	case TYPE_UNSIGNED:       RECV_OUTPUT_ARRAY(unsigned); break;
	case TYPE_LONG:           RECV_OUTPUT_ARRAY(long); break;
	case TYPE_UNSIGNED_LONG:  RECV_OUTPUT_ARRAY(unsigned long); break;
	case TYPE_FLOAT:          RECV_OUTPUT_ARRAY(float); break;
	case TYPE_DOUBLE:         RECV_OUTPUT_ARRAY(double); break;
	case TYPE_LONG_DOUBLE:    RECV_OUTPUT_ARRAY(long double); break;
	case TYPE_LONG_LONG_INT:  RECV_OUTPUT_ARRAY(long long int); break;
	}

	return 0;	
}

int _RecvInputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... ) {

	int i, value;
	va_list vl;

	int size_array[size_n], pattern_array[pattern_n];
	partitioned_dim--; // convert partition dimension to zero based

	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	return _RecvInputNDMsgCore(ptr, type_id, loop_start, loop_end, loop_step,
				partitioned_dim, pattern_type, size_n, pattern_n, size_array, pattern_array);
}

int _RecvInputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array )
{
	int start_offset, recv_size;
	int nelem_size = 1, block_size = 1;

	/*printf("LOOP_START = %d\n", loop_start);
	printf("LOOP_END = %d\n", loop_end);
	printf("LOOP_STEP = %d\n", loop_step);
	printf("PARTITION DIM = %d\n", partitioned_dim);
	printf("PATTERN TYPE = %d\n", pattern_type);
	printf("SIZE N = %d\n", size_n);
	printf("PATTERN N = %d\n", pattern_n);*/

	_CalcStartOffsetAndSize(&start_offset, &recv_size, loop_start, loop_end, loop_step,
							partitioned_dim, pattern_type,
							size_n, pattern_n, size_array, pattern_array);	

#if 0
	if (_gfn_rank == 0) {
		printf("START OFFSET = %d\n", start_offset);
		printf("RECV SIZE = %d\n", recv_size);
	}
#endif

#define RECV_INPUT_ARRAY(type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	_RecvWorkerMsg((void*)(tmp_ptr+start_offset), recv_size * sizeof(type)); \
} while(0)
	
	// _RecvWorkerMsg((void*)(((type*)ptr) + start_offset), recv_size * sizeof(type));

	switch(type_id)
	{
	case TYPE_CHAR:           RECV_INPUT_ARRAY(char); break;
	case TYPE_UNSIGNED_CHAR:  RECV_INPUT_ARRAY(unsigned char); break;
	case TYPE_SHORT:          RECV_INPUT_ARRAY(short); break;
	case TYPE_UNSIGNED_SHORT: RECV_INPUT_ARRAY(unsigned short); break;
	case TYPE_INT:            RECV_INPUT_ARRAY(int); break;
	case TYPE_UNSIGNED:       RECV_INPUT_ARRAY(unsigned); break;
	case TYPE_LONG:           RECV_INPUT_ARRAY(long); break;
	case TYPE_UNSIGNED_LONG:  RECV_INPUT_ARRAY(unsigned long); break;
	case TYPE_FLOAT:          RECV_INPUT_ARRAY(float); break;
	case TYPE_DOUBLE:         RECV_INPUT_ARRAY(double); break;
	case TYPE_LONG_DOUBLE:    RECV_INPUT_ARRAY(long double); break;
	case TYPE_LONG_LONG_INT:  RECV_INPUT_ARRAY(long long int); break;
	}

	return 0;	
}

int _SendOutputNDMsg(	void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, ... ) {

	int i, value;
	va_list vl;
	int start_offset, send_size;
	int nelem_size = 1, block_size = 1;

	int size_array[size_n], pattern_array[pattern_n];
	partitioned_dim--; // convert partition dimension to zero based

	// We want to pass size_n + pattern_n but it warning,
	// So pass pattern_n that add with size_n
	pattern_n += size_n;
	va_start(vl, pattern_n);
	pattern_n -= size_n;
	for (i = 0; i < size_n; ++i) size_array[i] = va_arg(vl, int);
	for (i = 0; i < pattern_n; ++i) pattern_array[i] = va_arg(vl, int);
	va_end(vl);

	return _SendOutputNDMsgCore(ptr, type_id, loop_start, loop_end, loop_step,
				partitioned_dim, pattern_type, size_n, pattern_n, size_array, pattern_array);
}

int _SendOutputNDMsgCore(void *ptr, int type_id,
						int loop_start, int loop_end, int loop_step,
						int partitioned_dim, int pattern_type,
						int size_n, int pattern_n, int *size_array, int *pattern_array )
{
	int start_offset, send_size;
	int nelem_size = 1, block_size = 1;

	_CalcStartOffsetAndSize(&start_offset, &send_size, loop_start, loop_end, loop_step,
							partitioned_dim, pattern_type,
							size_n, pattern_n, size_array, pattern_array);	

#if 1
	if (_gfn_rank == 0) {
		printf("START OFFSET = %d\n", start_offset);
		printf("SEND SIZE = %d\n", send_size);
	}
#endif

#define SEND_OUTPUT_ARRAY(type) \
do { \
	type * tmp_ptr = (type *) ptr; \
	_SendWorkerMsg((void*)(tmp_ptr+start_offset), send_size * sizeof(type)); \
} while(0)
	
	// _SendWorkerMsg((void*)(((type*)ptr) + start_offset), send_size * sizeof(type));

	switch(type_id)
	{
	case TYPE_CHAR:           SEND_OUTPUT_ARRAY(char); break;
	case TYPE_UNSIGNED_CHAR:  SEND_OUTPUT_ARRAY(unsigned char); break;
	case TYPE_SHORT:          SEND_OUTPUT_ARRAY(short); break;
	case TYPE_UNSIGNED_SHORT: SEND_OUTPUT_ARRAY(unsigned short); break;
	case TYPE_INT:            SEND_OUTPUT_ARRAY(int); break;
	case TYPE_UNSIGNED:       SEND_OUTPUT_ARRAY(unsigned); break;
	case TYPE_LONG:           SEND_OUTPUT_ARRAY(long); break;
	case TYPE_UNSIGNED_LONG:  SEND_OUTPUT_ARRAY(unsigned long); break;
	case TYPE_FLOAT:          SEND_OUTPUT_ARRAY(float); break;
	case TYPE_DOUBLE:         SEND_OUTPUT_ARRAY(double); break;
	case TYPE_LONG_DOUBLE:    SEND_OUTPUT_ARRAY(long double); break;
	case TYPE_LONG_LONG_INT:  SEND_OUTPUT_ARRAY(long long int); break;
	}

	return 0;
}

void _SendCallFuncMsg(int func_code) {
	_SendMasterMsg(&func_code, sizeof(int));
}

void _RecvCallFuncMsg(int *func_code) {
	_RecvWorkerMsg(func_code, 4);
}



void _SendMasterMsg(void *buffer, size_t size /*,status&*/) {
	unsigned offset = 0;
	size_t send_size = 0;
	size_t total_send_size = 0;

	while(offset < size) {
		size_t write_size = MIN(MAX_SOCKET_BUFFER_SIZE, size - offset);
		send_size = write(sockfd, buffer + offset, write_size);
		offset += MAX_SOCKET_BUFFER_SIZE;
		total_send_size += send_size;
	}
#ifdef IPC_DEBUG
	printf("Worker write buffer for %zu bytes\n", total_send_size);
#endif // IPC_DEBUG
}

void _RecvMasterMsg(void *buffer, size_t size /*, status&*/) {
	unsigned offset = 0;
	size_t recv_size = 0;
	size_t total_recv_size = 0;

	while(offset < size) {
		size_t read_size = MIN(MAX_SOCKET_BUFFER_SIZE, size - offset);
		recv_size = read(sockfd, buffer + offset, read_size /* Desired size */);
		offset += MAX_SOCKET_BUFFER_SIZE;
		total_recv_size += recv_size;
	}	
	if (total_recv_size != size) {
		fprintf(stderr, "Error: _RecvMasterMsg, Expected size is %zu but transfered size is %zu\n",
			size, total_recv_size);
	}
#ifdef IPC_DEBUG
	printf("Worker read buffer for %zu bytes\n", total_recv_size);
#endif // IPC_DEBUG
}

void _SendWorkerMsg(void *buffer, size_t size /*,status&*/) { 
	unsigned offset = 0;
	size_t send_size = 0;
	size_t total_send_size = 0;

	while(offset < size) {
		size_t write_size = MIN(MAX_SOCKET_BUFFER_SIZE, size - offset);
		send_size = write(connfd, buffer + offset, write_size);
		offset += MAX_SOCKET_BUFFER_SIZE;
		total_send_size += send_size;
	}
#ifdef IPC_DEBUG
	printf("Master write buffer for %zu bytes\n", total_send_size);
#endif // IPC_DEBUG
}

void _RecvWorkerMsg(void *buffer, size_t size /*, status&*/) {
	unsigned offset = 0;
	size_t recv_size = 0;
	size_t total_recv_size = 0;
	
	while(offset < size) {
		size_t read_size = MIN(MAX_SOCKET_BUFFER_SIZE, size - offset);
		recv_size = read(connfd, buffer + offset, read_size /* Desired size */);
		offset += MAX_SOCKET_BUFFER_SIZE;
		total_recv_size += recv_size;
	}	
	if (total_recv_size != size) {
		fprintf(stderr, "Error: _RecvWorkerMsg, Expected size is %zu but transfered size is %zu\n",
			size, recv_size);
	}
#ifdef IPC_DEBUG
	printf("Master read buffer for %zu bytes\n", total_recv_size);
#endif // IPC_DEBUG
}
