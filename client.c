#include "client.h"

int main(int argc, char **argv) {
	int go = 1;
	char *command;
	int port = PORT;
	int sSock, lSock;
	char input[USERIO];
	char *param, *path;
	char rBuffer[BUFSIZE];
	char *hostname = "localhost";
	struct sockaddr_in server_addr;
	struct addrinfo hints, *result;
	
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

	} else {
		printf(	"Error: Wrong number of arguments. Usage: file_client <dir> [hostname] [port].\n");
	}

	return 0;
}

void getFileList(int socket) {
	int bytesRead = 0;
	char rBuffer[BUFSIZE];

	send(socket, "DIR", CMD, 0);

	while ((bytesRead = recv(socket, rBuffer, BUFSIZE, 0)) == BUFSIZE) {
		rBuffer[bytesRead] = '\0';
		printf("%s\n", rBuffer);
	}
}

void listDirectory(int socket, char *path) {
	DIR *dfd;
	struct dirent *dp;
	char filename[256];
	struct stat statbuf;

	send(socket, "LST", CMD, 0);

	dfd = opendir(path);
	while ((dp = readdir(dfd)) != NULL) {
		sprintf(filename, "%s%s", path, dp->d_name);
		stat(filename, &statbuf);

		if ((statbuf.st_mode & S_IFMT) == S_IFREG) {
			send(socket, dp->d_name, BUFSIZE, 0);
		}
	}

	send(socket, "TRM", CMD, 0);
	printf("list of local files was sent to the server\n");

	closedir(dfd);
}

void getFile(int socket, char *path, char *filename) {
	int s;

	FILE *fp = NULL;
	char rBuffer[BUFSIZE];
	char *fullpath = NULL;

	int bytesRead = 0;
	int bytesWritten = 0;
	int totalBytesWritten = 0;

	struct sockaddr_in server_addr;

	assert((fullpath = (char *)malloc((strlen(path) + strlen(filename) + 1) * sizeof(char))) != NULL);
	strcpy(fullpath, path);
	strcat(fullpath, filename);

	fp = fopen(fullpath, "wb+");
	if (fp == NULL) {
		printf("Error: Couldn't open file.\n");
	} else {
		send(socket, "GET", CMD, 0);
		send(socket, filename, BUFSIZE, 0);

		bytesRead = recv(socket, rBuffer, CMD, 0);
		rBuffer[bytesRead] = '\0';
		if (strcmp(rBuffer, "ERR") == 0) {
			recv(socket, rBuffer, BUFSIZE, 0);
			printf("Server Error: %s.\n", rBuffer);
			remove(fullpath);
		} else {
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = inet_addr(rBuffer);
			recv(socket, rBuffer, BUFSIZE, 0);
			server_addr.sin_port = strToInt(rBuffer);

			assert(connect(s, (struct sockaddr*) &server_addr, sizeof(server_addr)) != -1);

			send(s, filename, BUFSIZE, 0);

			bzero(rBuffer, BUFSIZE);
			while ((bytesRead = recv(s, rBuffer, BUFSIZE, 0)) == BUFSIZE) {
				assert((bytesWritten = fwrite(rBuffer, sizeof(char), BUFSIZE, fp)) == BUFSIZE);
				totalBytesWritten += bytesWritten;
				bzero(rBuffer, BUFSIZE);
			}
			fclose(fp);

			if (strcmp(rBuffer, "ERR") == 0) {
				recv(socket, rBuffer, BUFSIZE, 0);
				printf("Peer Error: %s.\n", rBuffer);
				remove(fullpath);

				send(socket, "ERR", CMD, 0);
				send(socket, CLIENT_FILE_TRANSFER_FAILED, BUFSIZE, 0);
			} else {
				send(socket, "TRM", CMD, 0);
				printf("transfer complete\n");
			}

			close(s);
		}
	}

	free(fullpath);
}
