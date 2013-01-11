#include "hashmap.h"

hashMap createHash(int size, int (*hash)(void *)) {
	hashMap map;

	map.max_size = size;
	map.curr_size = 0;
	map.hash = hash;
	map.hashmap = (list **)malloc(size * sizeof(list *));

	return map;
}

void clearMap(hashMap* map)
{
	int i;

	for (i = 0; i < map->max_size; i++) {
		if (map->hashmap[i] != NULL)
			deleteList(map->hashmap[i]);
			free(map->hashmap[i]);
	}

	free (map->hashmap);
}

void put(hashMap *map, void *key, void *data) {
	int i = map->hash(key);

	if (map->hashmap[i] == NULL) map->hashmap[i] = createList();
	addFirst(map->hashmap[i], key, data);
	map->curr_size++;
}

listItem *get(hashMap *map, void *key)
{
	int i = map->hash(key);

	list *l = map->hashmap[i];
	return find(l, key);
}

void removeElement(hashMap *map, listItem *elm) {
	if (elm != NULL) {
		removeItem(elm);
		map->curr_size--;
	}
}

void remove(hashMap *map, char* key)
{
	listItem *elm = get(map, key);
	removeElement(map, elm);
}
