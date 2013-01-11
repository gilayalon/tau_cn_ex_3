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

listItem *createNew(void *key, void *data);
list *createList();
void addFirst(list *lst, void *key, void *data);
void addLast(list *lst, void *key, void *data);
void removeItem(listItem *item);
listItem *find(list *lst, void *key);
void delete(listItem *head);
void deleteList(list *lst);
int isEmpty(list *lst);

#endif
