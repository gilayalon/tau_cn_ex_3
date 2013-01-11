#include <sys/socket.h>
#include <arpa/inet.h>
#include "clients.h"
#include "files.h"

unsigned int id = 0;

int main() {
	file *test;
	char *filename = "README.txt";
	listItem *head;
	struct sockaddr_in peer;
	hashMap *files = createHash(HASH_SIZE, hash);

	file *f = createFile("README.txt", NULL);

	peer.sin_family = AF_INET;
	peer.sin_port = htons(10000);
	peer.sin_addr.s_addr = inet_addr("127.0.0.1");

	addClient(f->clients, createClient(id++, &peer));

	test = getFile(files, filename);

	printf("filename: %s\n", test->filename);
	printf("client list: \n");
	head = test->clients->head;
	for (; head != test->clients->tail; head = head->next)
		printf("client id: %d\n", (int)head->key);

	clearFiles(files);
	clearMap(files);
	free(files);

	return 0;
}
