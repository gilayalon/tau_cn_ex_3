#include "files.h"

file *createFile(char *filename, list *clients) {
	file *f = (file *)malloc(sizeof(file));

	f->filename = filename;

	if (clients != NULL) {
		f->clients = clients;
	} else {
		f->clients = createList();
	}

	return f;
}

void addFile(hashMap *files, file *f) {
	put(files, f->filename, f->clients);
}

void removeFile(hashMap *files, char *filename) {
	file *f = getFile(files, filename);

	if (f != NULL) {
		clearList(f->clients->head);
		free(f->clients);
		removeElement(files, f->filename);
	}
}

file *getFile(hashMap *files, char *filename) {
	listItem *f = get(files, filename);

	if (f != NULL) {
		return createFile((char *)f->key, (list *)f->data);
	}

	return NULL;
}

void clearFiles(hashMap *files) {
	int i;
	char **fileList = getFileList(files);

	for (i = 0; i < files->curr_size; i++) {
		removeFile(files, fileList[i]);
	}

	free(fileList);
}

char **getFileList(hashMap *files) {
	int i;
	int j = 0;
	listItem *head;

	char **fileList = (char **)malloc(files->curr_size * sizeof(char *));

	for (i = 0; i < HASH_SIZE; i++) {
		if (files->map[i] != NULL) {
			head = files->map[i]->head;
			for (; head != files->map[i]->tail; head = head->next) {
				fileList[j++] = (char *)head->key;
			}
		}
	}

	return fileList;
}

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
