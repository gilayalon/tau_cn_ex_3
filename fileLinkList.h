#ifndef FILELINKLIST_H_
#define FILELINKLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include "fileList.h"

typedef struct fileLink
{
	file *f;
	struct fileLink *prev;
	struct fileLink *next;
} fileLink;

typedef struct fileLinkList
{
	fileLink *head;
	fileLink *tail;
} fileLinkList;

fileLinkList *fll_create();
fileLink *fll_createItem(file *f);
void fll_add(fileLinkList *list, file *f);
void fll_clear(fileLink *head);
int fll_isEmpty(fileLinkList *list);


#endif
