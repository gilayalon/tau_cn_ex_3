#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

#include "mds.h"
#include "helpers.h"

#define CMD 3
#define PORT 2511
#define BUFSIZE 256
#define TCP_LISTEN_BACKLOG 50
#define FILE_NOT_FOUND_ERROR "Server Error: File not found on server."
#define SERVER_WELCOME_MESSAGE "Welcome! I am simple-file-server."

int id = 0;
int done = 0;
pthread_mutex_t m;
int task_for = -1;
fd_set active_fd_set;
pthread_cond_t pending_task_condition;

void listFiles(int socket);
void initFileTransaction(int socket, int client_id);
void registerClientFiles(int socket, int client_id);
void *client_thread(void *param);
int create_socket(unsigned short int port);

#endif
