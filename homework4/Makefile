CC = gcc
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm

explorer: explorer.o map.o

explorer.o: map.o explorer.c

map.o: map.c

clean :
	rm explorer map.o explorer.o