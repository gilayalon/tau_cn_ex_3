all: main

clean:
	-rm main.o main files.o clients.o list.o hashmap.o

main: main.o clients.o files.o
	gcc -pedantic-errors -g -lm -o main main.o clients.o files.o

main.o: main.c files.h clients.h
	gcc -pedantic-errors -g -lm -o main.c

files.o: files.c files.h list.h hashmap.h
	gcc -pedantic-errors -c -Wall -g files.c

clients.o: clients.c clients.h list.h hashmap.h
	gcc -pedantic-errors -c -Wall -g clients.c

list.o: list.c list.h
	gcc -pedantic-errors -c -Wall -g list.c

hashmap.o: hashmap.c hashmap.h
	gcc -pedantic-errors -c -Wall -g hashmap.c
