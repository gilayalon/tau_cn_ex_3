#ifndef MDS_H_
#define MDS_H_

#include "fileHash.h"
#include "clientHash.h"

fileHash *mds_files;
clientHash *mds_clients;

void mds_init();
void mds_put(client *c, file *f);
client *mds_get(file *f);
void mds_remove(client *c);
void mds_clear();

#endif
