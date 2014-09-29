INSTALL=..
LIB=$(INSTALL)/lib
INCLUDE=$(INSTALL)/include
CC=gcc
CFLAGS=-Wall -g -fPIC -shared -I$(INCLUDE)/
LFLAGS=-L$(LIB) -lgenericList

LIBOBJS=List.o Node.o

all: libs 

libs: libgenericList.so  libgenericList.a

libgenericList.so: $(LIBOBJS)
	$(LD) -shared -fPIC -o $@  $(LIBOBJS)

libgenericList.a: $(LIBOBJS)
	$(AR)  rcv $@ $(LIBOBJS)
	ranlib $@

install: libs
	install -m 644  List.h $(INCLUDE)/
	install -m 644  Node.h $(INCLUDE)/
	install -m 644  common.h $(INCLUDE)/
	install -m 755  libgenericList.so $(LIB)/libgenericList.so
	install -m 755  libgenericList.a $(LIB)/libgenericList.a
	
installclean:  clean
	rm -f $(LIB)/libgenericList.so
	rm -f $(LIB)/libgenericList.a
	rm -f $(INCLUDE)/List.h
	rm -f $(INCLUDE)/Node.h
	rm -f $(INCLUDE)/common.h
	
clean:
	rm -f list *.o a.out core libgenericList.a libgenericList.so
