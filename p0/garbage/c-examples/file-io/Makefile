
CC=gcc
CFLAGS=-Wall -g  


all: gentxt genbin filesize subdirs


gentxt: generate_text.o
	$(CC) $(CFLAGS)  -o $@ $<

genbin: generate_binary.o
	$(CC) $(CFLAGS)  -o $@ $<

filesize: filesize.o
	$(CC) $(CFLAGS)  -o $@ $<

subdirs:
	cd disk_search; make
	cd checkpoint; make
	cd checkpoint-java; make

clean:
	rm --force *.o a.out genbin gentxt  data* filesize
	cd disk_search; make clean
	cd checkpoint; make clean
	cd checkpoint-java; make clean

