all: library mydash

library:	
	cd list_src; make clean; make; cp lib/* ..;

mydash:
	cd src; make clean; make; cp mydash ..;

clean:
	rm mydash libgenericList.*; cd list_src; make clean; cd ../src; make clean;