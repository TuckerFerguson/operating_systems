
KERNEL_SOURCE=/lib/modules/`uname -r`/build
all:
	 make -Wdeclaration-after-statement -C ${KERNEL_SOURCE} M=`pwd` modules

obj-m += booga.o

clean:
	/bin/rm --force .booga* booga.o booga.mod.c booga.mod.o booga.ko Module.symvers modules.order
	/bin/rm -fr .tmp_versions/

test:
	gcc -g -Wall -o test-booga test-booga.c