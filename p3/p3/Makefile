CC=gcc
INSTALL=list
INCLUDE=$(INSTALL)/include -I.
LIBS=$(INSTALL)/lib 
LFLAGS=-L$(LIBS) -lpthread -lgenericList
CFLAGS=-Wall -g -I$(INCLUDE)
DEBUG=

all: library pc

library:
	cd list; make;

pc: pc.o  Item.o Item.h
	$(CC) $(CFLAGS) -o $@ pc.o Item.o  $(LFLAGS)

clean:
	cd list; make clean; cd ..; /bin/rm -f *.o pc
