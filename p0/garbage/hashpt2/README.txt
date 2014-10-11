*****************************
* Assignment 4              * 
* CompSci 253		        *
* April 23, 2013            *
* Reuben Tanner (113104237) *
*****************************

PROJECT OVERVIEW:

This project is a hash table that uses the hashing-by-chaining method with a doubly-linked linked list in each element of the hash table.

INCLUDED FILES:

 * wfmenu.c - source code

 * HashObject.c - source code
 * Node.c - source code
 * List.c - source code
 * HashTable.c - source code

 * HashObject.h - header file
 * Node.h - header file
 * List.h - header file
 * HashTable.h - header file

 * Makefile - makefile
 * README.txt - this file

BUILDING AND RUNNING:
All project files should be in the same directory.
	$./wfmenu 

TESTING:
	I tested my program by modifying the TestList.c class to make it compatible with the HashTable class. I changed the functions and the structure
	of the input to be conducive to use with the HashTable. Getting this class set up took a while but in the end it was worth it because when I would
	make changes, instead of going to my regular test class (how I normally test) and changing a whole bunch of things or adding new methods I just kept right
	on plugging away. It was time consuming but good. Since we are really only utilizing a few utility functions of the hash table, the testing wasn't too in depth,
	I made data validation checks and then passed it on down the line. Debugged it when it wasn't performing as it should have which was very frustrating at times
	because my GDB kept on crashing on my Fedora distro, allowing me to get through only a few steps before a crash. 

LEAKS:
	I used a lot of valgrind -v --leak-check=full and --tool=memcheck on this one. Before implementing the hash table, I thoroughly removed the memory leaks
	from the doubly linked list which proved very useful once I got to the hash table. I eventually learned to speak Valgrind and could "invalid read of length 1" 
	meant that I wasn't doing strings right which helped to eliminate a lot of crashing and a lot of problems right off the bat. I would run valgrind incrementally
	after I had made some progress on the HashTable and began to see the patterns of mallocing and freeing. Freeing something only once finally clicked for me also:
	if I create an object using malloc and insert it into the table and then free it, it will be removed from the table, what I really want to do is free the object
	that was inserted only when I free the table. The trickiest part for me with tracking memory leaks was freeing the space that was allocated for my struct list **,
	I had the program running completely clean except for 1,488 bytes in my createHashTable function and couldn't figure out what I had forgotten to free: I was iterating
	through each index of the double pointer and freeing everything but it was still messing up. Eventually after beating my head against the wall I just said "aw to heck with it, I'm just going to free the ** and see what happens" and then it worked. 

In case you noticed, yes, this the same README from last time...I got extra credit from the BST so I decided not to try too hard...sorry. Thanks for grading all our projects. Have a good summer.
