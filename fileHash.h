#ifndef FILEHASH_H_
#define FILEHASH_H_

/*
 * GENERAL:
 * We are using a file hash to support around 5000 files, the hash
 * is implemented with the chaining mechanism to avoid collisions,
 * each hashed element representing a file has a pointer to a linked
 * list holding the IP's of hosts that have a copy of the file in
 * their working directory. Having the file hash lets us search the
 * main data structure (MDS) in O(1) time complexity and get a host IP
 * if the file exists with almost no overhead.
 *
 */

#include "fileList.h"

#define FILE_HASH_MAX_ELEMENTS 5000
#define FILE_HASH_SIZE 997

typedef struct fileHash
{
	int max_size;	/* max number of files in the hash 		*/
	int curr_size;	/* current number of files in the hash 	*/
	fileList **map;	/* file linked list to avoid collisions */
} fileHash;

fileHash *fh_create();
file *fh_get(fileHash *hashmap, char *filename);
void fh_put(fileHash *hashmap, file *f);
void fh_remove(fileHash *hashmap, char *filename);
void fh_clear(fileHash *hashmap);
int fh_hash(char *filename);

#endif
