#include "fileList.h"

fileList *fl_create() {
	fileList *list = (fileList *)malloc(sizeof(fileList));

	list->head = fl_createItem("$", NULL);
	list->tail = fl_createItem("$", NULL);

	list->head->next = list->tail;
	list->tail->prev = list->head;

	return list;
}

file *fl_createItem(char *filename, clientLinkList *clients) {
	file *f = (file *)malloc(sizeof(file));

	f->filename = (char *)malloc(strlen(filename) * sizeof(char));
	strcpy(f->filename, filename);

	f->clients = clients;
	f->next = NULL;
	f->prev = NULL;

	return f;
}

file *fl_find(fileList *list, char *filename) {
	file *f;

	if (list != NULL) {
		f = list->head;

		for (; f != list->tail; f = f->next) {
			if (strcmp(f->filename, filename) == 0) return f;
		}
	}

	return NULL;
}

void fl_add(fileList *list, file *newFile) {
	newFile->next = list->head->next;
	newFile->prev = list->head;
	list->head->next->prev = newFile;
	list->head->next = newFile;
}

void fl_remove(file *f) {
	if (f != NULL) {
		f->prev->next = f->next;
		f->next->prev = f->prev;

		free(f->filename);
		if (f->clients != NULL) cll_clear(f->clients->head);

		free(f);
	}
}

void fl_clear(file *head) {
	if (head != NULL) {
		fl_clear(head->next);

		free(head->filename);
		if (head->clients != NULL) cll_clear(head->clients->head);

		free(head);
	}
}

int fl_isEmpty(fileList *list) {
	return (list->head->next == list->tail);
}
