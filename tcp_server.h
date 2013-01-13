#ifndef FILE_SERVER_H_
#define FILE_SERVER_H_

#define BUFSIZE 256
#define CMD 3

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "helpers.h"

int listDirectory(int socket, char *path);
int getFile(int socket, char *path);
int sendFile(int socket, char *path);

#endif
