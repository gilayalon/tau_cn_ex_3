#ifndef MDS_H_
#define MDS_H_
/*
 * GENERAL:
 * The main idean behind the Main Data Structure MDS is to hold two
 * seperate hash tables on for the connected clients and the other
 * for all available files. The two hashes are linked together such that
 * each file points to it's "Client-Link-List"(CLL) a list of clients that have
 * a copy of this file, while each client points to it's "File-Link-List"(FLL)
 * so when a client goes off-line the server can easily update the MDS and remove
 * the client and all it's files from the system.
 *
 * the use of two hashes make the computational complexity in the range of O(1)
 * for all operations. Note: we are using chaining to avoid collisions in the hash
 * currently we can support up-to 5000 different files form 500 different clients
 * registered in the MDS and keep a good QoS.
 */
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
