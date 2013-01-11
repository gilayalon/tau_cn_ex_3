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

file *createFile(char *filename, list *clients);
file *getFile(hashMap *files, char *filename);
void addFile(hashMap *files, file *f);
void removeFile(hashMap *files, char *filename);
void clearFiles(hashMap *files);

char **getFileList(hashMap *files);
int hash(void *key);

#endif
