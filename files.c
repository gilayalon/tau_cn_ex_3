#include "files.h"

int hash(void* key)
{
	int c;
	unsigned int hash = 5831;
	char *filename = (char *)key;

	while ((c = *filename++))
	{
		hash = ((hash << 5) + hash) + c; /* hash + 33 + c */
	}

	return (hash % HASH_SIZE);
}


