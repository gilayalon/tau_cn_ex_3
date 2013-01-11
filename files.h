#ifndef FILES_H_
#define FILES_H_

#include "list.h"
#include "hashmap.h"

#define MAX_ELEMENTS 5000
#define HASH_SIZE 997

typedef struct file
{
	char *filename;
	list *clients;
} file;

int hash(void *key);

#endif
