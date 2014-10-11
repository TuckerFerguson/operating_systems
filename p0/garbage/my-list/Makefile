INCLUDE=.
CC=gcc
#CFLAGS=-Wall -g -I$(INCLUDE)
CFLAGS=-Wall -g

maintest: maintest.o List.o Node.o
	$(CC) $(CFLAGS) -o $@ maintest.o List.o Node.o

clean:
	rm -f *.o maintest