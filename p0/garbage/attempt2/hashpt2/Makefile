INCLUDE=.
CC=gcc
#CFLAGS=-Wall -g -static  -I$(INCLUDE)
CFLAGS=-Wall -g  -I$(INCLUDE)

all: wfmenu

TestList: TestList.o HashObject.o List.o Node.o 
	$(CC) $(CFLAGS) -o $@ TestList.o HashObject.o List.o Node.o  $(LFLAGS)

SimpleTest: SimpleTest.o List.o Node.o 
	$(CC) $(CFLAGS) -o $@ SimpleTest.o List.o Node.o $(LFLAGS)

wfmenu: wfmenu.o HashTable.o HashObject.o List.o Node.o
	$(CC) $(CFLAGS) -o $@ wfmenu.o HashTable.o HashObject.o List.o Node.o $(LFLAGS)
	
run: wfmenu
	./wfmenu
	
clean:
	rm --force list *.o wfmenu

