all: file_server file_client

clean:
	-rm server.o file_server mds.o fileHash.o clientHash.o fileList.o clientList.o fileLinkList.o clientLinkList.o helpers.o
	-rm client.o file_client

file_server: server.o mds.o fileHash.o clientHash.o fileList.o clientList.o fileLinkList.o clientLinkList.o helpers.o
	gcc -pedantic-errors -g -lm -o file_server server.o mds.o fileHash.o clientHash.o fileList.o clientList.o fileLinkList.o clientLinkList.o helpers.o -lpthread

file_client: client.o helpers.o
	gcc -pedantic-errors -g -lm -o file_client client.o helpers.o

server.o: server.c server.h mds.h helpers.h
	gcc -pedantic-errors -c -Wall -g server.c

mds.o: mds.c mds.h fileHash.h clientHash.h
	gcc -pedantic-errors -c -Wall -g mds.c

fileHash.o: fileHash.c fileHash.h fileList.h
	gcc -pedantic-errors -c -Wall -g fileHash.c

clientHash.o: clientHash.c clientHash.h clientList.h
	gcc -pedantic-errors -c -Wall -g clientHash.c

fileList.o: fileList.c fileList.h clientLinkList.h
	gcc -pedantic-errors -c -Wall -g fileList.c

clientList.o: clientList.c clientList.h fileLinkList.h
	gcc -pedantic-errors -c -Wall -g clientList.c

fileLinkList.o: fileLinkList.c fileLinkList.h fileList.h
	gcc -pedantic-errors -c -Wall -g fileLinkList.c

clientLinkList.o: clientLinkList.c clientLinkList.h
	gcc -pedantic-errors -c -Wall -g clientLinkList.c

helpers.o: helpers.c helpers.h
	gcc -pedantic-errors -c -Wall -g helpers.c
