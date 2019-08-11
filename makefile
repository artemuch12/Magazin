all: main
main: main.o
	gcc main.o -o main -lpthread
main.o: main.c
	gcc -g main.c -c
