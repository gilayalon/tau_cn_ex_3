#ifndef CLIENTS_H_
#define CLIENTS_H_

#include "list.h"

#define MAX_ELEMENTS 100
#define HASH_SIZE 97

typedef struct hashItem
{
	unsigned int id;
	struct sockaddr_in *peer;
} hashItem;

int hash(const int id);

#endif /* CLIENTS_H_ */
