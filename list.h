#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct listItem
{
	void *key;
	void *data;
	struct listItem *prev;
	struct listItem *next;
} listItem;

typedef struct list
{
	listItem *head;
	listItem *tail;
} list;

list *createList();
listItem *createItem(void *key, void *data);
listItem *find(list *lst, void *key);
void addFirst(list *lst, void *key, void *data);
void addLast(list *lst, void *key, void *data);
void removeItem(listItem *item);
void clearList(listItem *head);
int isEmpty(list *lst);

#endif
