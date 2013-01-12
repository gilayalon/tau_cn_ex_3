#ifndef CLIENTHASH_H_
#define CLIENTHASH_H_

#include "clientList.h"

#define CLIENT_HASH_MAX_ELEMENTS 2000
#define CLIENT_HASH_SIZE 701

typedef struct clientHash
{
	int max_size;
	int curr_size;
	clientList **map;
} clientHash;

clientHash *ch_create();
client *ch_get(clientHash *hashmap, int id);
void ch_put(clientHash *hashmap, client *c);
void ch_remove(clientHash *hashmap, int id);
void ch_clear(clientHash *hashmap);
int ch_hash(int id);

#endif
