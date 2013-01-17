#ifndef CLIENT_H_
#define CLIENT_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include "helpers.h"

#define CMD 3
#define PORT 2511
#define USERIO 300
#define BUFSIZE 256
#define LISTEN_PORT 25110
#define TCP_LISTEN_BACKLOG 50
#define CLIENT_FILE_TRANSFER_FAILED "Client Error: file transfer failed."

char *path;
int done = 0;
pthread_mutex_t m;
int in_process = 0;
pthread_cond_t in_process_condition;
pthread_cond_t all_complete_condition;

void listDirectory(int socket, char *path);
void getFileList(int socket);
void getFile(int socket, char *path, char *filename);
void *serverThread();
void *sendFileThread(void *param);

#endif
