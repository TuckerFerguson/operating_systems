INSTALL=..
INCLUDE=$(INSTALL)/list_src/include -I.
LIBS=$(INSTALL) -lreadline -lncurses
LFLAGS=-L$(LIBS) -lgenericList
CC=gcc
CFLAGS=-Wall -g -I$(INCLUDE)

PROGS=mydash dox

all: $(PROGS) 

mydash: shell.o error.o CommandParser.o Job.o DirectoryTraverser.o JobManager.o
	$(CC) $(CFLAGS) -o $@ shell.o error.o CommandParser.o Job.o DirectoryTraverser.o JobManager.o $(LFLAGS)

commandparser: CommandParser.o TestCommandParser.o
	$(CC) $(CFLAGS) -o $@ CommandParser.o TestCommandParser.o
	
jobmanager: JobManager.o TestJobManager.o CommandParser.o error.o Job.o
	$(CC) $(CFLAGS) -o $@ JobManager.o TestJobManager.o CommandParser.o error.o Job.o $(LFLAGS)
	
version:
	cd src;./set_git_version.sh;

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config > doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

clean:
	/bin/rm -f *.o $(PROGS) a.out core  *.log
	/bin/rm -fr docs
