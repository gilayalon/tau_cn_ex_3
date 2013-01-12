#include "fileLinkList.h"

fileLinkList *fll_create() {
	fileLinkList *list = (fileLinkList *)malloc(sizeof(fileLinkList));

	list->head = fll_createItem(NULL);
	list->tail = fll_createItem(NULL);

	list->head->next = list->tail;
	list->tail->prev = list->head;

	return list;
}

fileLink *fll_createItem(file *f) {
	fileLink *link = (fileLink *)malloc(sizeof(fileLink));

	link->f = f;
	link->next = NULL;
	link->prev = NULL;

	return link;
}

void fll_add(fileLinkList *list, file *f) {
	fileLink *newLink = fll_createItem(f);

	newLink->next = list->head->next;
	newLink->prev = list->head;
	list->head->next->prev = newLink;
	list->head->next = newLink;
}

void fll_clear(fileLink *head) {
	if (head != NULL) {
		fll_clear(head->next);
		free(head);
	}
}

int fll_isEmpty(fileLinkList *list) {
	return (list->head->next == list->tail);
}
