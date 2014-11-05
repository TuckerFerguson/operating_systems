JFLAGS=
.SUFFIXES: .class .java
.java.class:
	 javac $(JFLAGS) $<

CC=gcc
CFLAGS= -g   -Wall 
#CFLAGS= -O  -Wall  
LIBS=-lpthread
PROGS=webstats  Webstats.class

all: $(PROGS)

webstats: webstats.o error.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ webstats.o error.o

Webstats: Webstats.class

clean:
	/bin/rm -f *.o $(PROGS) Webstats.class
