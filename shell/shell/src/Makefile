LIBS=-lgenericList -lreadline -lncurses
LFLAGS=-L$(LIBS)
CC=gcc
CFLAGS=-v -g -Wall

PROGS=shell

all: $(PROGS) dox

shell: shell.o
	$(CC) $(CFLAGS) -o $@  $< $(LIBS)

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
