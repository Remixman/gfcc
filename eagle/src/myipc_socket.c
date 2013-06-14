#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myipc.h"

// http://www.thegeekstuff.com/2011/12/c-socket-programming/

#define MIN(A,B) ((A<B)?(A):(B))
#define MAX(A,B) ((A>B)?(A):(B))

#define USED_PORT 5829
#define MAX_SOCKET_BUFFER_SIZE 4096
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


void _SendInputMsg(void *buffer, size_t size /*, status&*/) {
	_SendMasterMsg(buffer, size);
}

void _RecvOutputMsg(void *buffer, size_t size /*, status&*/) {
	_RecvMasterMsg(buffer, size);
}

void _RecvInputMsg(void *buffer, size_t size /*, status&*/) {
	_RecvWorkerMsg(buffer, size);
}

void _SendOutputMsg(void *buffer, size_t size /*, status&*/) {
	_SendWorkerMsg(buffer, size);
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

	while(offset < size) {
		size_t read_size = MIN(MAX_SOCKET_BUFFER_SIZE, size - offset);
		recv_size += read(sockfd, buffer + offset, read_size /* Desired size */);
		offset += MAX_SOCKET_BUFFER_SIZE;
	}	
	if (recv_size != size) {
		fprintf(stderr, "Error: _RecvMasterMsg, Expected size is %zu but transfered size is %zu\n",
			size, recv_size);
	}
#ifdef IPC_DEBUG
	printf("Worker read buffer for %zu bytes\n", size);
#endif // IPC_DEBUG
}

void _SendWorkerMsg(void *buffer, size_t size /*,status&*/) { 
	unsigned offset = 0;
	while(offset < size) {
		size_t write_size = MIN(MAX_SOCKET_BUFFER_SIZE, size - offset);
		write(connfd, buffer + offset, write_size);
		offset += MAX_SOCKET_BUFFER_SIZE;
	}
#ifdef IPC_DEBUG
	printf("Master write buffer for %zu bytes\n", size);
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
