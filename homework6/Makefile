CFLAGS = -g -Wall -std=c99

interpreter: interpreter.o core.o basic.o extra.o

interpreter.o: core.h basic.h extra.h

core.o: core.h

basic.o: basic.h core.h

extra.o: extra.h core.h

clean:
	rm -f *.o
	rm -f interpreter
