#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "list.h"

typedef struct hashMap
{
	int max_size;
	int curr_size;
	int (*hash)(void *);
	list **hashmap;
} hashMap;

hashMap createHash(int size, int (*hash)(void *));
void put(hashMap *map, void *key, void *data);
listItem *get(hashMap *map, void *key);
void removeElement(hashMap *map, listItem *elm);
void remove(hashMap *map, void  *key);
void clearMap(hashMap *map);

#endif
