#ifndef FILES_H_
#define FILES_H_

#include "list.h"
#include "hashmap.h"

#define MAX_ELEMENTS 5000
#define HASH_SIZE 997

typedef struct hashItem
{
	char *filename;
	hashmap *clients;
} hashItem;

int hash(const char* filename);

#endif
