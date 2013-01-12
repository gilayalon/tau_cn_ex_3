#include "clientLinkList.h"

clientLinkList *cll_create() {
	clientLinkList *list = (clientLinkList *)malloc(sizeof(clientLinkList));

	list->head = cll_createItem(NULL);
	list->tail = cll_createItem(NULL);

	list->head->next = list->tail;
	list->tail->prev = list->head;

	return list;
}

clientLink *cll_createItem(struct client *c) {
	clientLink *link = (clientLink *)malloc(sizeof(clientLink));

	link->c = c;
	link->next = NULL;
	link->prev = NULL;

	return link;
}

void cll_addFirst(clientLinkList *list, struct client *c) {
	clientLink *newLink = cll_createItem(c);

	if (list == NULL) list = cll_create();

	newLink->next = list->head->next;
	newLink->prev = list->head;
	list->head->next->prev = newLink;
	list->head->next = newLink;
}

void cll_addLast(clientLinkList *list, struct client *c) {
	clientLink *newLink = cll_createItem(c);

	if (list == NULL) list = cll_create();

	newLink->next = list->tail;
	newLink->prev = list->tail->prev;
	list->tail->prev->next = newLink;
	list->tail->prev = newLink;
}

void cll_remove(clientLink *link) {
	if (link != NULL) {
		link->prev->next = link->next;
		link->next->prev = link->prev;

		free(link);
	}
}

void cll_clear(clientLink *head) {
	if (head != NULL) {
		cll_clear(head->next);
		free(head);
	}
}

int cll_isEmpty(clientLinkList *list) {
	return (list->head->next == list->tail);
}
