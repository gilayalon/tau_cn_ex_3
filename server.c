#include "server.h"

/*
 * Create a new thread to handle a client's communication:
 * DIR: get a list of available files
 * GET <filename>: get a host's IP that has a copy of the file
 * LST: update the server's registry (MDS) with this client's files
 * QUT: this client is turning off-line, clear this client's file records from the MDS
 */

void *client_thread(void *param) {
	char rBuffer[CMD];
	int connected = 1;
	int id = *(int *)param;
	int cSock = mds_get_client(id)->socket_fd;

	while (!done && connected) {
		assert(pthread_mutex_lock(&m) == 0);
		assert(pthread_cond_wait(&pending_task_condition, &m) == 0);
		assert(pthread_mutex_unlock(&m) == 0);

		if (cSock == task_for) {
			recv(cSock, rBuffer, CMD, 0);
			rBuffer[CMD] = '\0';

			if (strcmp(trim(rBuffer), "DIR") == 0) {
				listFiles(cSock);
			} else if (strcmp(trim(rBuffer), "GET") == 0) {
				initFileTransaction(cSock, id);
			} else if (strcmp(trim(rBuffer), "LST") == 0) {
				registerClientFiles(cSock, id);
			} else if (strcmp(trim(rBuffer), "QUT") == 0) {
				send(cSock, "TRM", CMD, 0);
				close(cSock);
				mds_remove(mds_get_client(id));

				assert(pthread_mutex_lock(&m) == 0);
				FD_CLR(cSock, &active_fd_set);
				assert(pthread_mutex_unlock(&m) == 0);

				connected = 0;
			}
		}
	}

	return NULL;
}

/*
 * Return a list of all files available throughout the
 * entire sytem (i.e. Main-Server, client1, client2, ... , clientN
 */
void listFiles(int socket) {
	int i;
	char **fileList = mds_get_file_list();

	for (i = 0; i < mds_get_size(); i++) {
		send(socket, fileList[i], BUFSIZE, 0);
	}

	send(socket, "TRM", CMD, 0);
	free(fileList);
}

/*
 * When a client sends a GET <filename> request the server
 * searches the MDS to return a host that has a copy of the
 * requested file, this information is then sent back to the
 * client so he (the client) can connect to the host and get
 * the actual copy of the file. once the transfer is complete
 * the server updates it's records (MDS) that the client now
 * has a copy of the file and he can now direct traffic to
 * that client.
 */
void initFileTransaction(int socket, int client_id) {
	client *c;
	int bytesRead = 0;
	char filename[BUFSIZE];
	char rBuffer[BUFSIZE];

	bytesRead = recv(socket, rBuffer, BUFSIZE, 0);
	rBuffer[bytesRead] = '\0';
	strcpy(filename, rBuffer);
	memset(&rBuffer, 0, sizeof(rBuffer));

	c = mds_get(filename);
	if (c != NULL) {
		send(socket, inet_ntoa(c->address->sin_addr), BUFSIZE, 0);
		sprintf(rBuffer, "%d", c->address->sin_port);
		send(socket, rBuffer, BUFSIZE, 0);

		bytesRead = recv(socket, rBuffer, CMD, 0);
		rBuffer[bytesRead] = '\0';
		if (strcmp(rBuffer, "ERR") == 0) {
			recv(socket, rBuffer, BUFSIZE, 0);
		} else {
			assert(pthread_mutex_lock(&m) == 0);
			mds_put(mds_get_client(client_id), mds_get_file(filename));
			assert(pthread_mutex_unlock(&m) == 0);
		}
	} else {
		send(socket, "ERR", CMD, 0);
		send(socket, FILE_NOT_FOUND_ERROR, BUFSIZE, 0);
	}
}

/*
 * When a new client connects to the server first we need to update
 * the server's data structure (MDS) with the files and associate them
 * to the specific client. This is done with the use of mds_put.
 * since the server is multi-threaded we need to exercise thread safety
 * there for the lock is needed when updateing the MDS
 */
void registerClientFiles(int socket, int client_id) {
	file *f;
	int bytesRead = 0;
	char rBuffer[BUFSIZE];

	client *c = mds_get_client(client_id);

	while ((bytesRead = recv(socket, rBuffer, BUFSIZE, 0)) == BUFSIZE) {
		rBuffer[bytesRead] = '\0';
		f = fl_createItem(rBuffer, NULL);

		assert(pthread_mutex_lock(&m) == 0);
		mds_put(c, f);
		assert(pthread_mutex_unlock(&m) == 0);
	}
}

/*
 * Return a socket for TCP on the given port
 */
int create_socket(unsigned short int port) {
	  int s;
	  struct sockaddr_in name;

	  assert((s = socket (AF_INET, SOCK_STREAM, 0)) != -1);

	  name.sin_family = AF_INET;
	  name.sin_port = htons(port);
	  name.sin_addr.s_addr = htonl(INADDR_ANY);

	  assert((bind(s, (struct sockaddr *)&name, sizeof(name)) != -1));

	  return s;
}


/*
 * The server listen's on the given port, when a client connects it
 * sends a list of all available files in it's working direcroty to
 * the server, the sever registers the files in it's data structure
 * later on request it can retrieve the requested file's host thus
 * letting the client connect to the host to get the requested file.
 *
 * The server handles each request with a different thread, when the
 * thread's work is done it's automatically cleared from memory.
 */
int main(int argc, char **argv) {
	int i;
	client *c;
	size_t size;
	int port = PORT;
	int lSock, cSock;
	pthread_t cThread;
	struct sockaddr_in clientAddress;
	fd_set read_fd_set;

    assert(pthread_mutex_init(&m, NULL) == 0);
	assert(pthread_cond_init(&pending_task_condition, NULL) == 0);

	if (argc == 2) {
		port = strToInt(argv[1]);
		if (port == -1) {
			printf("Server Error: Invalid port number. Switching back to default (%d).\n", PORT);
			port = PORT;
		}
	}

	lSock = create_socket(port);
	assert(listen(lSock, TCP_LISTEN_BACKLOG) != -1);

	FD_ZERO (&active_fd_set);
	FD_SET (lSock, &active_fd_set);

	mds_init();

	while (1) {
		read_fd_set = active_fd_set;

		assert(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) != -1);
		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_fd_set)) {
			    if (i == lSock) {
			    	size = sizeof(clientAddress);

			    	assert((cSock = accept(lSock, (struct sockaddr *)&clientAddress, &size)) != -1);

					assert(pthread_mutex_lock(&m) == 0);

			    	FD_SET(cSock, &active_fd_set);

			    	id++;
			    	c = cl_createItem(id, cSock, &clientAddress, NULL);
			    	mds_put_client(c);

					assert(pthread_mutex_unlock(&m) == 0);

			    	assert(pthread_create(&cThread, NULL, client_thread, &id) == 0);
			    	sleep(2);

			    	send(cSock, SERVER_WELCOME_MESSAGE, BUFSIZE, 0);
			    } else {
			    	task_for = i;
			    	assert(pthread_cond_broadcast(&pending_task_condition) == 0);
				}
			}
		}
	}

	done = 1;
	assert(pthread_cond_broadcast(&pending_task_condition) == 0);
	assert(pthread_mutex_unlock(&m) == 0);

	sleep(2);
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&pending_task_condition);

	mds_clear();

	return 0;
}
