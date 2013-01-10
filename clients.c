#include "clients.h"

int hash(const int id)
{
	return (id % HASH_SIZE);
}
