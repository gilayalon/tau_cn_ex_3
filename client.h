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
#define LISTEN_PORT 25110
#define USERIO 300
#define BUFSIZE 256
#define CLIENT_FILE_TRANSFER_FAILED "Client Error: file transfer failed."

int done = 0;
pthread_mutex_t m;
pthread_cond_t pending_task_condition;

void listDirectory(int socket, char *path);
void getFileList(int socket);
void getFile(int socket, char *path, char *filename);

#endif
