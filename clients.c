#include "clients.h"

client *createClient(unsigned int id, struct sockaddr_in *address) {
	client *c = (client *)malloc(sizeof(client));

	c->id = id;
	c->address = address;

	return c;
}

void addClient(list *clients, client *c) {
	addLast(clients, &(c->id), c->address);
}

void removeClient(list *clients, unsigned int id) {
	listItem *c = find(clients, &id);
	removeItem(c);
}

client *getClient(list *clients, unsigned int id) {
	listItem *c = find(clients, &id);
	return createClient((unsigned int)c->key, (struct sockaddr_in *)c->data);
}
