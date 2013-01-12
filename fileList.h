#ifndef FILELIST_H_
#define FILELIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientLinkList.h"

typedef struct file
{
	char *filename;
	clientLinkList *clients;
	struct file *prev;
	struct file *next;
} file;

typedef struct fileList
{
	file *head;
	file *tail;
} fileList;

fileList *fl_create();
file *fl_createItem(char *filename, clientLinkList *clients);
file *fl_find(fileList *list, char *filename);
void fl_add(fileList *list, file *newFile);
void fl_remove(file *f);
void fl_clear(file *head);
int fl_isEmpty(fileList *list);

#endif
