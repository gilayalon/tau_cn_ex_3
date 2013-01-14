#include "mds.h"

void mds_init() {
	mds_files = fh_create();
	mds_clients = ch_create();
}

void mds_put(client *c, file *f) {
	file *fp = fh_get(mds_files, f->filename);
	client *cp = ch_get(mds_clients, c->id);

	if (cp == NULL && fp == NULL) {
		ch_put(mds_clients, c);
		fh_put(mds_files, f);

		c->files = fll_create();
		f->clients = cll_create();

		fll_add(c->files, f);
		cll_addFirst(f->clients, c);

	} else if (cp == NULL && fp != NULL) {
		ch_put(mds_clients, c);

		c->files = fll_create();

		fll_add(c->files, fp);
		cll_addFirst(fp->clients, c);

	} else if (cp != NULL && fp == NULL) {
		fh_put(mds_files, f);

		f->clients = cll_create();

		fll_add(cp->files, f);
		cll_addFirst(f->clients, cp);

	} else {
		fll_add(cp->files, fp);
		cll_addFirst(fp->clients, cp);
	}
}

void mds_put_client(client *c) {
	if (ch_get(mds_clients, c->id) == NULL) {
		ch_put(mds_clients, c);
		c->files = fll_create();
	}
}

client *mds_get(char *filename) {
	clientLink *cl;
	client *c = NULL;
	file *f = fh_get(mds_files, filename);

	if (f != NULL) {
		cl = f->clients->head->next;
		c = cl->c;
		cll_remove(cl);
		cll_addLast(f->clients, c);
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

void mds_clear() {
	fh_clear(mds_files);
	ch_clear(mds_clients);

	free(mds_files);
	free(mds_clients);
}

int mds_get_size() {
	return mds_files->curr_size;
}
