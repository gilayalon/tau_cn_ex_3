#include "mds.h"

/*
 * Create the two main hashes "file hash" and "client hash"
 */
void mds_init() {
	mds_files = fh_create();
	mds_clients = ch_create();
}

/*
 * Add a client C and file F to the MDS and link them together.
 * Note: it's possible that the client is either exits or the
 * file f already exists, this is all taken into account in this
 * method. if they exist add the record, if they are new to the
 * MDS add an create the appropriate links.
 */
void mds_put(client *c, file *f) {
	file *fp = fh_get(mds_files, f->filename);
	client *cp = ch_get(mds_clients, c->id);
	/* new client & new file */
	if (cp == NULL && fp == NULL) {
		ch_put(mds_clients, c);
		fh_put(mds_files, f);

		c->files = fll_create();
		f->clients = cll_create();

		fll_add(c->files, f);
		cll_addFirst(f->clients, c);
	/* new client & existing file */
	} else if (cp == NULL && fp != NULL) {
		ch_put(mds_clients, c);

		c->files = fll_create();

		fll_add(c->files, fp);
		cll_addFirst(fp->clients, c);
	/* existing client & new file */
	} else if (cp != NULL && fp == NULL) {
		fh_put(mds_files, f);

		f->clients = cll_create();

		fll_add(cp->files, f);
		cll_addFirst(f->clients, cp);
	/*existing client & existing file */
	} else {
		fll_add(cp->files, fp);
		cll_addFirst(fp->clients, cp);
	}
}

/*
 * Add a new client to the MDS
 */
void mds_put_client(client *c) {
	if (ch_get(mds_clients, c->id) == NULL) {
		ch_put(mds_clients, c);
		c->files = fll_create();
	}
}

/*
 * Return a host (client) that has the file <filename>
 * in it's working directory. if no such client exists
 * the method returns NULL.
 *
 * Note: We have implemented a dynamic queue to huristically
 * balance the load on the hosts, a host that has been picked
 * is pushed to the end of the queue. this is not optimal but
 * under standard conditions it gives good results most of the
 * time.
 */
client *mds_get(char *filename) {
	clientLink *cl;
	client *c = NULL;
	file *f = fh_get(mds_files, filename);

	if (f != NULL) {
		cl = f->clients->head->next;
		c = cl->c;
		cll_remove(cl);				/* rotate the hosts (clients) list */
		cll_addLast(f->clients, c);	/* for load balancing */
		return c;
	}

	return c;
}

client *mds_get_client(int id) {
	return ch_get(mds_clients, id);
}

file *mds_get_file(char *filename) {
	return fh_get(mds_files, filename);
}

/*
 * Returns a list of all file names available throughout the entire MDS
 */
char **mds_get_file_list() {
	int i;
	file *f;
	int j = 0;


	char **fileList = (char **)malloc(mds_files->curr_size * sizeof(char *));

	for (i = 0; i < mds_files->max_size; i++) {
		if (mds_files->map[i] != NULL) {
			f = mds_files->map[i]->head->next;
			for (; f != mds_files->map[i]->tail; f = f->next) {
				fileList[j++] = f->filename;
			}
		}
	}

	return fileList;
}

/*
 * Removes Client c form the MDS, this method automatically updates both
 * the "client hash" and the "file hash" - removing "c" as a possible
 * host of it's files. if a certain file had only "c" as a sourch the file
 * is also removed from the MDS.
 *
 * Note: since we are using the linked hashes the computational complexity
 * of this operation is in directo proportion to to the number of files "c"
 * was hosting.
 */
void mds_remove(client *c) {
	file *f;
	clientLink *cl;
	fileLink *iter;

	if (ch_get(mds_clients, c->id) != NULL) {
		iter = c->files->head->next;
		for (; iter != c->files->tail; iter = iter->next) {
			f = iter->f;
			cl = f->clients->head;
			for (; cl != f->clients->tail; cl = cl->next) {
				if (cl->c == c) {
					cll_remove(cl);
				}
			}

			if (cll_isEmpty(f->clients)) {
				fh_remove(mds_files, f->filename);
			}
		}

		ch_remove(mds_clients, c->id);
	}
}

/*
 * clearing the memory before shutting down the server.
 */
void mds_clear() {
	fh_clear(mds_files);
	ch_clear(mds_clients);

	free(mds_files);
	free(mds_clients);
}

/*
 * Return the number of files currently listed in the server's
 * Main Data Structure (MDS)
 */
int mds_get_size() {
	return mds_files->curr_size;
}
