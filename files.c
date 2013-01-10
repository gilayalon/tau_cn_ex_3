#include "files.h"

int hash(const char* filename)
{
	int c;
	unsigned int hash = 5831;

	while ((c = *filename++))
	{
		hash = ((hash << 5) + hash) + c; /* hash + 33 + c */
	}

	return (hash % HASH_SIZE);
}


