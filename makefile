all: server

clean:
	-rm server.o server mds.o fileHash.o clientHash.o fileList.o clientList.o fileLinkList.o clientLinkList.o

server: server.o mds.o fileHash.o clientHash.o fileList.o clientList.o fileLinkList.o clientLinkList.o
	gcc -pedantic-errors -g -lm -o server server.o mds.o fileHash.o clientHash.o fileList.o clientList.o fileLinkList.o clientLinkList.o

server.o: server.c server.h mds.h
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
