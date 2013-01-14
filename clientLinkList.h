#ifndef CLIENTLINKLIST_H_
#define CLIENTLINKLIST_H_

#include <stdio.h>
#include <stdlib.h>

struct client;

typedef struct clientLink
{
	struct client *c;
	struct clientLink *prev;
	struct clientLink *next;
} clientLink;

typedef struct clientLinkList
{
	clientLink *head;
	clientLink *tail;
} clientLinkList;

clientLinkList *cll_create();
clientLink *cll_createItem(struct client *c);
void cll_addFirst(clientLinkList *list, struct client *c);
void cll_addLast(clientLinkList *list, struct client *c);
void cll_remove(clientLink *link);
void cll_clear(clientLink *head);
int cll_isEmpty(clientLinkList *list);

#endif
