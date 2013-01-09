/*
 * HashMap.h
 *
 *  Created on: Jan 9, 2013
 *      Author: avi
 */

#ifndef HASHMAP_H_
#define HASHMAP_H_
#define MAX_ELEMENTS 5000
#define HASH_SIZE (MAX_ELEMENTS / 5)

typedef int list_elem;

/*
 * hash element has:
 */
typedef struct _hash_elem
{
	list_elem* val_lst;
}HashElement;


typedef struct _hash_map
{
	int max_size;
	int curr_size;
	HashElement* hashmap;
}HashMap;

int init(HashMap* map);




#endif /* HASHMAP_H_ */
