#include "fileHash.h"

fileHash *fh_create() {
	fileHash *hashmap = (fileHash *)malloc(sizeof(fileHash));

	hashmap->max_size = FILE_HASH_SIZE;
	hashmap->curr_size = 0;
	hashmap->map = (fileList **)malloc(FILE_HASH_SIZE * sizeof(fileList *));

	return hashmap;
}

file *fh_get(fileHash *hashmap, char *filename) {
	int index = fh_hash(filename);
	fileList *list = hashmap->map[index];

	return fl_find(list, filename);
}

void fh_put(fileHash *hashmap, file *f) {
	int index = fh_hash(f->filename);

	if (hashmap->map[index] == NULL) hashmap->map[index] = fl_create();
	fl_add(hashmap->map[index], f);
	hashmap->curr_size++;
}

void fh_remove(fileHash *hashmap, char *filename) {
	file *file = fh_get(hashmap, filename);

	if (file != NULL) {
		fl_remove(file);
		hashmap->curr_size--;
	}
}

void fh_clear(fileHash *hashmap) {
	int i;

	for (i = 0; i < hashmap->max_size; i++) {
		if (hashmap->map[i] != NULL)
			fl_clear(hashmap->map[i]->head);
	}

	free(hashmap->map);
}

int fh_hash(char *filename) {
	int c;
	unsigned int hash = 5831;

	while ((c = *filename++))
	{
		hash = ((hash << 5) + hash) + c;
	}

	return (hash % FILE_HASH_SIZE);
}
