#ifndef CLIENTHASH_H_
#define CLIENTHASH_H_

/*
 * GENERAL:
 * We are using a client hash to support up-to 2000 clients, the hash
 * is implemented with the chaining mechanism to avoid collisions,
 * each hashed element representing a client has a pointer to a linked
 * list each pointing to a filename in the file-hash. Storing the clients
 * in a hash table allows us update the system when a client goes off-line
 * in time complexity of O(1).
 *
 */

#include "clientList.h"

#define CLIENT_HASH_MAX_ELEMENTS 2000
#define CLIENT_HASH_SIZE 701

typedef struct clientHash
{
	int max_size;		/* max number of clients the hash can support */
	int curr_size;		/* cueent number of clients registered in the hash */
	clientList **map;	/* list of clients to avoid collisions due to the hash function */
} clientHash;

clientHash *ch_create();
client *ch_get(clientHash *hashmap, int id);
void ch_put(clientHash *hashmap, client *c);
void ch_remove(clientHash *hashmap, int id);
void ch_clear(clientHash *hashmap);
int ch_hash(int id);

#endif
