#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "list.h"

typedef struct hashMap
{
	int max_size;
	int curr_size;
	int (*hash)(void *);
	list **map;
} hashMap;

hashMap *createHash(int size, int (*hash)(void *));
listItem *get(hashMap *hashmap, void *key);
void put(hashMap *hashmap, void *key, void *data);
void removeElement(hashMap *hashmap, void *key);
void clearMap(hashMap *hashmap);

#endif
