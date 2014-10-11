
CC=gcc
CFLAGS=-Wall -g -O -I. 
LFLAGS=

OBJECTS=List.o Node.o Job.o

all: TestList 


TestList: TestList.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ TestList.o $(OBJECTS) $(LFLAGS)


clean:
	rm -f TestList a.out core *.o
