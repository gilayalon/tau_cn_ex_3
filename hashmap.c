#include "list.h"
#include "hashmap.h"

/*
 * DJB2 - Hash function implementation
 */
int hash(const char* str)
{
	unsigned int hash = 5831;
	int c;
	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c; /* hash + 33 + c */
	}
	return hash;
}


int init(HashMap* map)
{
	map->hashmap = (HashMap*)malloc(HASH_SIZE * sizeof(HashElement));
	if ( map->hashmap )
	{
		map->max_size = HASH_SIZE;
		map->curr_size = 0;
		return 0;
	}
	return -1;
}

int hash_map_free(HashMap* map)
{
	int i;
	int max = map->max_size; /*TODO: might want to change to curr_size??? */
	for (i = 0; i < max; ++i) {
		ListFree( map->hashmap[i].val_lst);
	}
	free (map->hashmap);
	return 0;
}

int put(HashMap* map, char* key, HashElement* elem)
{
	int i = hash(key);
	/* I assume that 'ListAppend' will return 0 for success */
	if ( ! map->hashmap[i].ListAppend(elem) )
	{
		map->curr_size += 1;
		return 0;
	}
	return -1;
}
int get(HashMap* map, char* key, HashElement* value)
{
	int i = hash(key);
	if ( map->hashmap[i].val_lst )
	{
		/*search in short list if not found return -1 as well*/
		return 0;
	}
	return -1;
}
int remove(HashMap* map, char* key)
{
	int i = hash(key);
	if ( map->hashmap[i].val_lst )
	{
		/*remove key from val_lst */
		map->curr_size -= 1;
		return 0
	}
	return -1;
}
