#include "tcp_server.h"

int main(int argc, char **argv) {
	char *path;
	int port = 2511;
	int lSock, cSock;
	int connected = 0;
	unsigned int client;

	char rBuffer[CMD];
	struct sockaddr_in server_addr, client_addr;

	/* User specified port */
	if (argc == 3) {
		port = strToInt(argv[2]);
		if (port == -1)
			printf("Error: Invalid port number.\n");
	}

	/* User default port: 2511 */
	if (argc >= 2) {
		/* Set server directory for file download \ upload */
		path = argv[1];

		/* Define listening socket */
		lSock = socket(PF_INET, SOCK_STREAM, 0);

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		bind(lSock, (struct sockaddr *)&server_addr, sizeof(server_addr));
		listen(lSock, 5);

		while (1) {
			/* Connect client - Create client socket and send welcome message */
			client = sizeof(client_addr);
			cSock = accept(lSock, (struct sockaddr*)&client_addr, &client);
			send(cSock, "Welcome! I am simple-file-server.", BUFSIZE, 0);
			connected = 1;

			while (connected) {
				/* Get command from client */
				recv(cSock, rBuffer, CMD, 0);
				rBuffer[CMD] = '\0';

				if (strcmp(trim(rBuffer), "DIR") == 0) {
					/* List files in the server directory */
					listDirectory(cSock, path);
				} else if (strcmp(trim(rBuffer), "GET") == 0) {
					/* Send a file to the client */
					sendFile(cSock, path);
				} else if (strcmp(trim(rBuffer), "PUT") == 0) {
					/* Receive file from client */
					getFile(cSock, path);
				} else if (strcmp(trim(rBuffer), "QUT") == 0) {
					/* Close connection to client */
					send(cSock, "TRM", CMD, 0);
					close(cSock);
					connected = 0;
				}
			}
		}

	} else {
		printf("Error: Wrong number of arguments. Usage: file_server <dir> [port].\n");
	}

	return 0;
}

int listDirectory(int socket, char *path) {
	DIR *dfd;
	struct dirent *dp;
	char filename[256];
	struct stat statbuf;

	/* Open specified directory and read contents */
	dfd = opendir(path);
	while ((dp = readdir(dfd)) != NULL) {
		sprintf(filename, "%s%s", path, dp->d_name);
		stat(filename, &statbuf);

		/* List only regular files; Do not include sub-directories \ other file types */
		if ((statbuf.st_mode & S_IFMT) == S_IFREG) {
			send(socket, dp->d_name, BUFSIZE, 0);
		}
	}
	/* End Transmission */
	send(socket, "TRM", CMD, 0);

	closedir(dfd);

	return 0;
}

int getFile(int socket, char *path) {
	FILE *fp = NULL;
	char rBuffer[BUFSIZE];
	char *fullpath = NULL;

	int bytesRead = 0;
	int bytesWritten = 0;
	int totalBytesWritten = 0;

	/* Get filename from client */
	bytesRead = recv(socket, rBuffer, BUFSIZE, 0);
	rBuffer[bytesRead] = '\0'; /* Terminate String */

	/* Save file absolute path */
	assert((fullpath = (char *)malloc((strlen(path) + strlen(rBuffer) + 1) * sizeof(char))) != NULL);
	strcpy(fullpath, path);
	strcat(fullpath, rBuffer);

	/* Try opening the file */
	fp = fopen(fullpath, "wb+");
	if (fp == NULL) {
		/* Send error message to client */
		send(socket, "ERR", CMD, 0);
		send(socket, strerror(errno), BUFSIZE, 0);
	} else {
		/* Send Start-File-Upload (SFU) message to client */
		send(socket, "SFU", CMD, 0);

		/* Get file contents from client and save to the specified location */
		bzero(rBuffer, BUFSIZE);
		while ((bytesRead = recv(socket, rBuffer, BUFSIZE, 0)) == BUFSIZE) {
			bytesWritten = fwrite(rBuffer, sizeof(char), bytesRead, fp);
			totalBytesWritten += bytesWritten;
			bzero(rBuffer, BUFSIZE);
		}
		fclose(fp);

		/* Client reported an error during the upload process; Delete file from server */
		if (strcmp(rBuffer, "ERR") == 0) {
			recv(socket, rBuffer, BUFSIZE, 0);
			remove(fullpath);
		}

		/* End Transmission */
		send(socket, "TRM", CMD, 0);
	}

	free(fullpath);
	return totalBytesWritten;
}

int sendFile(int socket, char *path) {
	FILE *fp = NULL;
	struct stat statbuf;
	char rBuffer[BUFSIZE];
	char *fullpath = NULL;

	int bytesRead = 0;
	int totalBytesRead = 0;

	/* Get filename from client */
	bytesRead = recv(socket, rBuffer, BUFSIZE, 0);
	rBuffer[bytesRead] = '\0'; /* Terminate String */

	/* Save file absolute path */
	assert((fullpath = (char *)malloc((strlen(path) + strlen(rBuffer) + 1) * sizeof(char))) != NULL);
	strcpy(fullpath, path);
	strcat(fullpath, rBuffer);

	/* Try opening the file */
	fp = fopen(fullpath, "rb");
	if (fp == NULL) {
		/* Send error message to client */
		send(socket, "ERR", CMD, 0);
		send(socket, strerror(errno), BUFSIZE, 0);
	} else {
		/* Get file info */
		stat(fullpath, &statbuf);

		/* Read file contents and send it to the client */
		bzero(rBuffer, BUFSIZE);
		while ((bytesRead = fread(rBuffer, sizeof(char), BUFSIZE, fp)) > 0) {
			send(socket, rBuffer, BUFSIZE, 0);
			totalBytesRead += bytesRead;
			bzero(rBuffer, BUFSIZE);
		}
		fclose(fp);

		/* Make sure no error occurred during the reading process; Compare number of bytes read and sent */
		if (totalBytesRead != statbuf.st_size) {
			/* Send error message to client */
			send(socket, "ERR", CMD, 0);
			send(socket, strerror(errno), BUFSIZE, 0);
		} else {
			/* End Transmission */
			send(socket, "TRM", CMD, 0);
		}
	}

	free(fullpath);
	return totalBytesRead;
}
