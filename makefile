all: test

clean:
	-rm test.o test files.o clients.o list.o hashmap.o

test: test.o files.o clients.o hashmap.o list.o
	gcc -pedantic-errors -g -lm -o test test.o files.o clients.o hashmap.o list.o

test.o: test.c files.h clients.h
	gcc -pedantic-errors -c -Wall -g test.c

files.o: files.c files.h list.h hashmap.h
	gcc -pedantic-errors -c -Wall -g files.c

clients.o: clients.c clients.h list.h
	gcc -pedantic-errors -c -Wall -g clients.c

hashmap.o: hashmap.c hashmap.h list.h
	gcc -pedantic-errors -c -Wall -g hashmap.c

list.o: list.c list.h
	gcc -pedantic-errors -c -Wall -g list.c
