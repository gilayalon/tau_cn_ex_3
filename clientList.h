#ifndef CLIENTLIST_H_
#define CLIENTLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "fileLinkList.h"

typedef struct client
{
	int id;
	int thread_fd;
	int socket_fd;
	struct sockaddr_in *address;
	fileLinkList *files;
	struct client *prev;
	struct client *next;
} client;

typedef struct clientList
{
	client *head;
	client *tail;
} clientList;

clientList *cl_create();
client *cl_createItem(int id, int thread_fd, int socket_fd, struct sockaddr_in *address, fileLinkList *files);
client *cl_find(clientList *list, int id);
void cl_add(clientList *list, client *newClient);
void cl_remove(client *c);
void cl_clear(client *head);
int cl_isEmpty(clientList *list);

#endif
