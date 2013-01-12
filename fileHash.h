#ifndef FILEHASH_H_
#define FILEHASH_H_

#include "fileList.h"

#define FILE_HASH_MAX_ELEMENTS 5000
#define FILE_HASH_SIZE 997

typedef struct fileHash
{
	int max_size;
	int curr_size;
	fileList **map;
} fileHash;

fileHash *fh_create();
file *fh_get(fileHash *hashmap, char *filename);
void fh_put(fileHash *hashmap, file *f);
void fh_remove(fileHash *hashmap, char *filename);
void fh_clear(fileHash *hashmap);
int fh_hash(char *filename);

#endif
