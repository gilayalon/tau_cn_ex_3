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

typedef struct fileHash
{
	hashMap *map;
} fileHash;

int hash(void *key);
file *createFile(char *filename);
fileHash *initFiles();
void addFile(fileHash *files, file *f);
void removeFile(fileHash *files, char *filename);
file *getFile(fileHash *files, char *filename);
char **getFileList(fileHash *files);
void deleteFiles(fileHash *files);

#endif
