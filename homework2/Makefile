CC = gcc
CFLAGS = -g -Wall -std=c99
LDLIBS = -lm

all: comments mandelbrot

comments: comments.o

mandelbrot: mandelbrot.o

clean:
	rm -f output.txt
	rm -f comments mandelbrot
	rm -f comments.o mandelbrot.o
