#ifndef CLIENTS_H_
#define CLIENTS_H_

#include <netinet/in.h>
#include "list.h"

typedef struct client
{
	unsigned int id;
	struct sockaddr_in *address;
} client;

typedef struct clientList
{
	list *lst;
} clientList;

client *createClient(unsigned int id, struct sockaddr_in *address);
clientList *initClients();
void addClient(clientList *clients, client *c);
void removeClient(clientList *clients, unsigned int id);
client *getClient(clientList *clients, unsigned int id);
int isClientListEmpty(clientList *clients);

#endif /* CLIENTS_H_ */
