#ifndef MDS_H_
#define MDS_H_

#include "fileHash.h"
#include "clientHash.h"

fileHash *mds_files;
clientHash *mds_clients;

void mds_init();
void mds_put(client *c, file *f);
void mds_put_client(client *c);
client *mds_get(char *filename);
client *mds_get_client(int id);
file *mds_get_file(char *filename);
char **mds_get_file_list();
void mds_remove(client *c);
void mds_clear();
int mds_get_size();

#endif
