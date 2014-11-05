JFLAGS=

# Recognize files with .class and .java extensions
.SUFFIXES: .class .java
#
# # This is a rule to convert a file with .java extension
# # into a file with a .class extension. The macro $< just
# # supplies the name of the file (without the extension) 
# # that invoked this rule.
#
.java.class:
	 javac $(JFLAGS) $<

CC=gcc
LFLAGS=-lpthread
CFLAGS=-Wall -g 
DEBUG=

all: driver Driver

driver: driver.o file_access_monitor.o
	$(CC) $(CFLAGS) -o $@ file_access_monitor.o driver.o

Driver: Driver.class
		
clean:
	/bin/rm -f *.o driver *.class
