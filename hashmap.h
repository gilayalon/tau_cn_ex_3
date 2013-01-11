#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "list.h"

typedef struct hashItem
{
	list *item;
} hashItem;

typedef struct hashMap
{
	int max_size;
	int curr_size;
	int (*hash)(void *);
	hashItem *hashmap;
} hashMap;

hashMap init(int size, int (*hash)(void *));
void put(hashMap *map, void *key, void *data);
void *get(hashMap *map, void *key);
void remove(hashMap *map, void  *key);
void clearMap(hashMap *map);

#endif
