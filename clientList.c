#include "clientList.h"

clientList *cl_create() {
	clientList *list = (clientList *)malloc(sizeof(clientList));

	list->head = cl_createItem(-1, -1, -1, NULL, NULL);
	list->tail = cl_createItem(-1, -1, -1, NULL, NULL);

	list->head->next = list->tail;
	list->tail->prev = list->head;

	return list;
}

client *cl_createItem(int id, int thread_fd, int socket_fd, struct sockaddr_in *address, fileLinkList *files) {
	client *c = (client *)malloc(sizeof(client));

	c->id = id;
	c->thread_fd = thread_fd;
	c->socket_fd = socket_fd;

	if (address != NULL) {
		c->address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
		memcpy(c->address, address, sizeof(struct sockaddr_in));
	} else {
		c->address = NULL;
	}

	c->files = files;
	c->next = NULL;
	c->prev = NULL;

	return c;
}

client *cl_find(clientList *list, int id) {
	client *c;

	if (list != NULL) {
		c = list->head;

		for (; c != list->tail; c = c->next) {
			if (c->id == id) return c;
		}
	}

	return NULL;
}

void cl_add(clientList *list, client *newClient) {
	newClient->next = list->head->next;
	newClient->prev = list->head;
	list->head->next->prev = newClient;
	list->head->next = newClient;
}

void cl_remove(client *c) {
	if (c != NULL) {
		c->prev->next = c->next;
		c->next->prev = c->prev;

		free(c->address);
		if (c->files != NULL) fll_clear(c->files->head);

		free(c);
	}
}

void cl_clear(client *head) {
	if (head != NULL) {
		cl_clear(head->next);

		free(head->address);
		if (head->files != NULL) fll_clear(head->files->head);

		free(head);
	}
}

int cl_isEmpty(clientList *list) {
	return (list->head->next == list->tail);
}
