
KERNEL_SOURCE=/lib/modules/`uname -r`/build
all:
	 make -C ${KERNEL_SOURCE} M=`pwd` modules

obj-m += example.o

clean:
	/bin/rm --force example.o example.mod.c example.mod.o example.ko Module.symvers modules.order
	/bin/rm --force .example.*
	/bin/rm -fr .tmp_versions/



