#include "files.h"

int hash(void* key)
{
	int c;
	unsigned int hash = 5831;
	char *filename = (char *)key;

	while ((c = *filename++))
	{
		hash = ((hash << 5) + hash) + c; /* hash + 33 + c */
	}

	return (hash % HASH_SIZE);
}

file *createFile(char *filename) {
	file *f = (file *)malloc(sizeof(file));

	f->filename = filename;
	f->clients = initClients();

	return f;
}

fileHash *initFiles() {
	fileHash *map = (fileHash *)malloc(sizeof(fileHash));

	map->map = createHash(HASH_SIZE, hash);

	return map;
}

void addFile(fileHash *files, file *f) {
	put(files->map, f->filename, f->clients);
}

void removeFile(fileHash *files, char *filename) {
	file f = getFile(files->map, filename);

	if (f != NULL) {
		deleteList(f->clients);
		removeElement(files->map, f);
	}
}

file *getFile(fileHash *files, char *filename) {
	listItem temp = get(files->map, filename);

	if (temp != NULL) {
		return createFile(temp.key, temp.data);
	}

	return NULL;
}

char **getFileList(fileHash *files) {
	int i, j;
	listItem head;
	char **flist = (char **)malloc(files->map->curr_size * sizeof(char *));

	j = 0;
	for (i = 0; i < HASH_SIZE; i++) {
		if (files->map->hashmap[i] != NULL) {
			head = files->map->hashmap[i]->head;
			for (; head != files->map->hashmap[i]->tail; head = head->next) {
				flist[j++] = (char *)head->key;
			}
		}
	}

	return flist;
}

void deleteFiles(fileHash *files) {
	int i;
	char **flist = getFileList(files);

	for (i = 0; i < files->map->curr_size; i++) {
		removeFile(files, flist[i]);
	}

	free(flist);
	clearHash(files);
}
