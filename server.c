#include "server.h"

int main() {
	int i;
	char **flist;

	file *f1 = fl_createItem("README.txt", NULL);
	file *f2 = fl_createItem("TEST.jpg", NULL);
	file *f3 = fl_createItem("WALLPAPER.png", NULL);

	client *c1 = cl_createItem(101, 0, 0, NULL, NULL);
	client *c2 = cl_createItem(102, 0, 0, NULL, NULL);
	client *c3 = cl_createItem(103, 0, 0, NULL, NULL);

	mds_init();

	mds_put(c1, f1);
	mds_put(c1, f2);
	mds_put(c2, f1);
	mds_put(c2, f3);
	mds_put(c3, f1);
	mds_put(c3, f3);

	printf("possible client for file %s = %d\n", f1->filename, mds_get(f1)->id);
	printf("possible client for file %s = %d\n", f1->filename, mds_get(f1)->id);
	printf("possible client for file %s = %d\n", f1->filename, mds_get(f1)->id);
	printf("possible client for file %s = %d\n", f1->filename, mds_get(f1)->id);
	printf("possible client for file %s = %d\n", f3->filename, mds_get(f3)->id);
	printf("possible client for file %s = %d\n", f3->filename, mds_get(f3)->id);
	printf("possible client for file %s = %d\n", f3->filename, mds_get(f3)->id);

	flist = mds_get_file_list();
	for (i = 0; i < mds_get_size(); i++) {
		printf("%s\n", flist[i]);
	}
	free(flist);

	mds_clear();

	return 0;
}
