#ifndef CLIENTS_H_
#define CLIENTS_H_

#include <netinet/in.h>
#include "list.h"

typedef struct client
{
	unsigned int id;
	struct sockaddr_in *address;
} client;

client *createClient(unsigned int id, struct sockaddr_in *address);
client *getClient(list *clients, unsigned int id);
void addClient(list *clients, client *c);
void removeClient(list *clients, unsigned int id);

#endif /* CLIENTS_H_ */
