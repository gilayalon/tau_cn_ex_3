#include "client.h"


/*
 * The thread method to send a file form the current peer
 * another peer in the swarm
 *
 * we keep a global pointer "in_process" to indicate how many
 * file transfers are currently under way. This is imporantant
 * before we terminate the current peer.
 */
void *sendFileThread(void *param) {
	FILE* fp;
	char *fullpath;
	char sBuffer[BUFSIZE];
	char filename[BUFSIZE];
	int bytesRead, bytesSent;
	int socket = *(int*)param;

	assert(pthread_mutex_lock(&m) == 0);
	in_process++;
	assert(pthread_mutex_unlock(&m) == 0);

	recv(socket,filename, BUFSIZE, 0);

	fullpath = (char *)malloc((strlen(path) + strlen(filename) + 1) * sizeof(char));
	strcpy(fullpath, path);
	strcat(fullpath, filename);

	bytesSent = 0;
	fp = fopen(fullpath, "r");

	bzero(sBuffer, BUFSIZE);
	while ((bytesRead = fread(sBuffer, sizeof(char), BUFSIZE, fp) > 0)) {
		bytesSent += send(socket, sBuffer, BUFSIZE, 0);
		bzero(sBuffer, BUFSIZE);
	}
	send (socket, "TRM", CMD, 0);
	free (fullpath);

	assert(pthread_mutex_lock(&m) == 0);
	in_process--;
	assert(pthread_mutex_unlock(&m) == 0);

	if (in_process == 0) {
		assert(pthread_cond_signal(&in_process_condition) == 0);
	}

	return NULL;
}

/*
 * Client's "Server's thread", each client has it's own listening socket
 * on which it accepts GET <filename> requests form other peer. we have
 * decided to implement this behavior in a multi-threaded kind of way where
 * each connection is assigned is't own unique thread which is later
 * terminated at the end of the connection
 */
void *serverThread(void *param) {
	int lSock, cSock;
	pthread_t cThread;
	unsigned int client;
	struct timeval timeout;
	fd_set read_fd_set, active_fd_set;
	struct sockaddr_in server_addr, client_addr;
	unsigned short int port = *(unsigned short int *)param;

	lSock = socket(PF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(lSock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(lSock, TCP_LISTEN_BACKLOG);

	FD_ZERO (&active_fd_set);
	FD_SET (lSock, &active_fd_set);

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	while (!done) {
		read_fd_set = active_fd_set;
		assert(select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout) != -1);

		if (FD_ISSET(lSock, &read_fd_set)) {
			client = sizeof(client_addr);
			cSock = accept(lSock, (struct sockaddr*)&client_addr, &client);
			assert(pthread_create(&cThread, NULL, sendFileThread, &cSock) == 0);
			sleep(2);
		}
	}

	if (in_process > 0) {
		assert(pthread_mutex_lock(&m) == 0);
		assert(pthread_cond_wait(&in_process_condition, &m) == 0);
		assert(pthread_mutex_unlock(&m) == 0);
	}
	/* wait for all file transfers to complete before sutdown */
	assert(pthread_cond_signal(&all_complete_condition) == 0);

	return NULL;
}

/*
 * main client method, this method keeps getting user input for
 * files, sends the requests to the main server which then replises
 * from here on end, the current client connects to another peer in
 * the swarm to get he actual file.
 *
 * this method also invokes a listening thread to listen on a specific
 * socket "LISTENING_SOCK" and then accepts connections on that socket
 * and transfers files to other peers in the swarm.
 */

int main(int argc, char **argv) {
	int go = 1;
	char *param;
	char *command;
	int port = PORT;
	int sSock, lSock;
	pthread_t sThread;
	char input[USERIO];
	char rBuffer[BUFSIZE];
	unsigned short int lport;
	char *hostname = "localhost";
	struct sockaddr_in server_addr;
	struct addrinfo hints, *result;
	
    assert(pthread_mutex_init(&m, NULL) == 0);
	assert(pthread_cond_init(&in_process_condition, NULL) == 0);
	assert(pthread_cond_init(&all_complete_condition, NULL) == 0);

	if (argc == 4) {
		port = strToInt(argv[3]);
		if (port == -1) {
			printf("Server Error: Invalid port number. Switching back to default (%d).\n", PORT);
			port = PORT;
		}
	}

	if (argc >= 3) hostname = argv[2];

	if (argc >= 2) {
		path = argv[1];

		sSock = socket(PF_INET, SOCK_STREAM, 0);

		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;
		getaddrinfo(hostname, NULL, &hints, &result);

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr = ((struct sockaddr_in *) (result->ai_addr))->sin_addr;

		assert(connect(sSock, (struct sockaddr*) &server_addr, sizeof(server_addr)) != -1);
		recv(sSock, rBuffer, BUFSIZE, 0);
		printf("%s\n", rBuffer);
		bzero(rBuffer, BUFSIZE);
		
		listDirectory(sSock, path);

		lport = getListeningPort(sSock);
		assert(pthread_create(&sThread, NULL, serverThread, &lport) == 0);

		while (go) {
			if (fgets(input, USERIO, stdin) != NULL) {
				if (isEmpty(input)) {
					printf("Error: Command must have at least one character.\n");
				} else if (input[strlen(input) - 1] != '\n') {
					printf("Error: Line too long.\n");
				} else {
					input[strlen(input) - 1] = '\0';

					command = trim(strtok(input, " \t\v\f\r"));

					if (strcmp(command, "DIR") == 0) {
						if (strtok(NULL, " \t\v\f\r") != '\0') {
							printf("Error: Invalid command. Usage: DIR\n");
						} else {
							getFileList(sSock);
						}

					} else if (strcmp(command, "GET") == 0) {
						param = trim(strtok(NULL, " \t\v\f\r"));
						if (param == '\0' || strtok(NULL, " \t\v\f\r") != '\0') {
							printf("Error: Invalid command. Usage: GET <filename>\n");
						} else {
							getFile(sSock, path, param);
						}

					/* Command: QUIT - Close connection */
					} else if (strcmp(command, "QUIT") == 0) {
						if (strtok(NULL, " \t\v\f\r") != '\0') {
							printf("Error: Invalid command. Usage: QUIT\n");
						} else {
							send(sSock, "QUT", CMD, 0);
							recv(sSock, rBuffer, CMD, 0);
							if (strcmp(rBuffer, "TRM") == 0) go = 0;
						}

					} else {
						printf(
							"Error: Invalid command. Supported: DIR, GET and QUIT.\n");
					}
				}
			}
		}

		freeaddrinfo(result);
		done = 1;

		assert(pthread_mutex_lock(&m) == 0);
		assert(pthread_cond_wait(&all_complete_condition, &m) == 0);
		assert(pthread_mutex_unlock(&m) == 0);

		pthread_mutex_destroy(&m);
		pthread_cond_destroy(&in_process_condition);
		pthread_cond_destroy(&all_complete_condition);

	} else {
		printf(	"Error: Wrong number of arguments. Usage: file_client <dir> [hostname] [port].\n");
	}

	return 0;
}

/*
 * get a file list form the server of all available files
 */
void getFileList(int sock) {
	int bytesRead = 0;
	char rBuffer[BUFSIZE];

	send(sock, "DIR", CMD, 0);

	while ((bytesRead = recv(sock, rBuffer, BUFSIZE, 0)) == BUFSIZE) {
		rBuffer[bytesRead] = '\0';
		printf("%s\n", rBuffer);
	}
}

/*
 * On initial connection to the server, send this client's working
 * dirctory's files to the main server.
 */
void listDirectory(int sock, char *path) {
	DIR *dfd;
	struct dirent *dp;
	char filename[256];
	struct stat statbuf;

	send(sock, "LST", CMD, 0);

	dfd = opendir(path);
	while ((dp = readdir(dfd)) != NULL) {
		sprintf(filename, "%s%s", path, dp->d_name);
		stat(filename, &statbuf);

		if ((statbuf.st_mode & S_IFMT) == S_IFREG) {
			send(sock, dp->d_name, BUFSIZE, 0);
		}
	}

	send(sock, "TRM", CMD, 0);
	printf("list of local files was sent to the server\n");

	closedir(dfd);
}

/*
 * GET a certain file form a peer in the swarm, i.e. connect to
 * the main server to get the i.p. then determine the peer's
 * listening port number and get the file from the peer. When
 * the transfer completes send message to the sever to update
 * the records to show the file is also available form this client.
 */
void getFile(int sock, char *path, char *filename) {
	int s;
	int rc;
	FILE *fp = NULL;
	int finished = 0;
	int bytesRead = 0;
	int bytesWritten = 0;
	char rBuffer[BUFSIZE];
	char *fullpath = NULL;
	struct sockaddr_in server_addr;

	assert((fullpath = (char *)malloc((strlen(path) + strlen(filename) + 1) * sizeof(char))) != NULL);
	strcpy(fullpath, path);
	strcat(fullpath, filename);

	send(sock, "GET", CMD, 0);
	send(sock, filename, BUFSIZE, 0);

	bytesRead = recv(sock, rBuffer, BUFSIZE, 0);
	rBuffer[bytesRead] = '\0';
	if (strcmp(rBuffer, "ERR") == 0) {
		recv(sock, rBuffer, BUFSIZE, 0);
		printf("%s\n", rBuffer);
	} else {
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(rBuffer);
		memset(&rBuffer, 0, sizeof(rBuffer));

		recv(sock, rBuffer, BUFSIZE, 0);
		server_addr.sin_port = htons(strToInt(rBuffer) * 10);

		if (server_addr.sin_port != getListeningPort(sock)) {
			s = socket(PF_INET, SOCK_STREAM, 0);
			rc = connect(s, (struct sockaddr*) &server_addr, sizeof(server_addr));

			if (rc != -1) {
				fp = fopen(fullpath, "wb+");
				if (fp == NULL) {
					printf("Error: Couldn't open file.\n");
				} else {
					send(s, filename, BUFSIZE, 0);

					bzero(rBuffer, BUFSIZE);
					while ((bytesRead = recv(s, rBuffer, BUFSIZE, 0)) == BUFSIZE) {
						assert((bytesWritten = fwrite(rBuffer, sizeof(char), BUFSIZE, fp)) == BUFSIZE);
						bzero(rBuffer, BUFSIZE);
					}
					fclose(fp);
				}
			} else {
				printf("Error: couldn't connect to peer.\n");
			}

			close(s);
		} else {
			printf("Error: file already exists in client folder.\n");
		}

		send(sock, "TRM", CMD, 0);
		printf("transfer complete\n");
	}

	free(fullpath);
}

/*
 * Get the listening port of the peer, when running the system
 * over different IP's there is no problem but when both server
 * and peers are all hosted on the same IP we need a way to get
 * the dynamically allocated listening port to connect for file
 * transfer.
 */
unsigned short int getListeningPort(int sock) {
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	assert(getsockname(sock, (struct sockaddr *)&sin, &len) != -1);
	return ntohs(sin.sin_port * 10);
}
