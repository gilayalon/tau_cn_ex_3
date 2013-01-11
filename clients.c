#include "clients.h"

client *createClient(unsigned int id, struct sockaddr_in *address) {
	client *c = (client *)malloc(sizeof(client));

	c->id = id;
	c->address = address;

	return c;
}

clientList *initClients() {
	clientList *l = (clientList *)malloc(sizeof(clientList));

	l->lst = createList();

	return l;
}

void addClient(clientList *clients, client *c) {
	addLast(clients->lst, c->id, c->address);
}

void removeClient(clientList *clients, unsigned int id) {
	listItem temp = find(clients->lst, id);
	removeItem(temp);
}

client *getClient(clientList *clients, unsigned int id) {
	listItem temp = find(clients->lst, id);
	return createClient(temp.key, temp.data);
}

int isClientListEmpty(clientList *clients) {
	return isEmpty(clients->lst);
}
