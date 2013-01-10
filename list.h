#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct listItem
{
	void *data;
	struct listItem *prev;
	struct listItem *next;
} listItem;

typedef struct list
{
	listItem *head;
	listItem *tail;
} list;

listItem *createNew(void *data);
list createList();
void addItem(list *lst, void *data);
void removeItem(listItem *item);
void delete(listItem *head);
void deleteList(list *lst);

#endif
