#include "clientHash.h"

clientHash *ch_create() {
	clientHash *hashmap = (clientHash *)malloc(sizeof(clientHash));

	hashmap->max_size = CLIENT_HASH_SIZE;
	hashmap->curr_size = 0;
	hashmap->map = (clientList **)malloc(CLIENT_HASH_SIZE * sizeof(clientList *));

	return hashmap;
}

client *ch_get(clientHash *hashmap, int id) {
	int index = ch_hash(id);
	clientList *list = hashmap->map[index];

	return cl_find(list, id);
}

void ch_put(clientHash *hashmap, client *c) {
	int index = ch_hash(c->id);

	if (hashmap->map[index] == NULL) hashmap->map[index] = cl_create();
	cl_add(hashmap->map[index], c);
	hashmap->curr_size++;
}

void ch_remove(clientHash *hashmap, int id) {
	client *client = ch_get(hashmap, id);

	if (client != NULL) {
		cl_remove(client);
		hashmap->curr_size--;
	}
}

void ch_clear(clientHash *hashmap) {
	int i;

	for (i = 0; i < hashmap->max_size; i++) {
		if (hashmap->map[i] != NULL)
			cl_clear(hashmap->map[i]->head);
	}

	free(hashmap->map);
}

int ch_hash(int id) {
	return id % CLIENT_HASH_SIZE;
}
