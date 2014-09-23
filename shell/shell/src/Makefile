INSTALL=..
LIBS=$(INSTALL)/lib/* -lgenericList -lreadline -lncurses
LFLAGS=-L$(LIBS)
CC=gcc
CFLAGS=-g -Wall 

PROGS=test-readline loop 

all: $(PROGS) dox

loop: loop.o
	$(CC) $(CFLAGS) -o $@  $<

test-readline: test-readline.o
	$(CC) $(CFLAGS) -o $@  $< $(LIBS)

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config > doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"


clean:
	/bin/rm -f *.o $(PROGS) a.out core  *.log
	/bin/rm -fr docs
