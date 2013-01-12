
#ifndef FDS_H_
#define FDS_H_

#include "hashmap.h"
#include "list.h"
#include "Client.h"		/*should have a client struct i.e. client id, client addr_in, list of pointers to it's file records*/
#include "fileRecord.h" 	/*should have a file record struct i.e. file name and list of clients */

/*
 * We will hold two hashs one for the files and one for clients, we will link the client with it's files to enable fast client removal
 */
int FDS_Put(Client c, fileRecord f);
/*
 * search the "file hash" to get a random client, if the file doesn't exist return NULL
 * if the fileRecord has no clients (see FDS_Remove) remove it from the hash "on the fly"
 */
int FDS_Get(fileRecord f);

/*
 * Removes the Client C and all it's files from the FDS, we find the client and iterate on it's fireRecords list removing each file
 * note: this can result in the file with no clients, we have to check that when we return a fileRecord
 */
int FDS_Remove(Client c);



#endif /* FDS_H_ */
