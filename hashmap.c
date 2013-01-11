#include "hashmap.h"

hashMap *createHash(int size, int (*hash)(void *)) {
	hashMap *hashmap = (hashMap *)malloc(sizeof(hashMap));

	hashmap->max_size = size;
	hashmap->curr_size = 0;
	hashmap->hash = hash;
	hashmap->map = (list **)malloc(size * sizeof(list *));

	return hashmap;
}

void clearMap(hashMap* hashmap)
{
	int i;

	for (i = 0; i < hashmap->max_size; i++) {
		if (hashmap->map[i] != NULL)
			clearList(hashmap->map[i]->head);
			free(hashmap->map[i]);
	}

	free (hashmap->map);
}

void put(hashMap *hashmap, void *key, void *data) {
	int i = hashmap->hash(key);

	if (hashmap->map[i] == NULL) hashmap->map[i] = createList();
	addFirst(hashmap->map[i], key, data);
	hashmap->curr_size++;
}

listItem *get(hashMap *hashmap, void *key)
{
	int i = hashmap->hash(key);
	list *l = hashmap->map[i];

	return find(l, key);
}

void removeElement(hashMap *hashmap, void *key) {
	listItem *elm = get(hashmap, key);

	if (elm != NULL) {
		removeItem(elm);
		hashmap->curr_size--;
	}
}
