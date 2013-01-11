#include "hashmap.h"

hashMap init(int size, int (*hash)(void *)) {
	hashMap map;

	map.max_size = size;
	map.curr_size = 0;
	map.hash = hash;
	map.hashmap = (hashItem *)malloc(size * sizeof(hashItem));

	return map;
}

void clearMap(hashMap* map)
{
	int i;

	for (i = 0; i < map->max_size; i++) {
		if (map->hashmap[i]->item != NULL)
			deleteList(map->hashmap[i]->item);
			free(map->hashmap[i]->item);
	}

	free (map->hashmap);
}

void put(hashMap *map, void *key, void *data) {
	int i = map->hash(key);

	if (map->hashmap[i]->item == NULL) map->hashmap[i]->item = createList();
	addFirst(map->hashmap[i]->item, key, data);
	map->curr_size++;
}

void *get(hashMap *map, void *key)
{
	int i = map->hash(key);

	list *l = map->hashmap[i]->item;
	return find(l, key);
}

void remove(hashMap* map, char* key)
{
	listItem temp = get(map, key);
	if (temp != NULL) {
		removeItem(temp);
		map->curr_size--;
	}
}
