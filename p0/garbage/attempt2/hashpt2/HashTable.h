#ifndef __HASHTABLE_H
#define __HASHTABLE_H
#include "List.h"
#include "HashObject.h"

typedef struct HashTable HashTable;
typedef struct HashTable * HashTablePtr;

struct HashTable {
	int tableSize;
	int (*getKey)(void *);
	char *  (*toString)(void *);
	void (*freeHashObject)(void *);
	Boolean (*compare)(void *, void *);
	ListPtr *table;
	Boolean maxChainReached;
};

void CondenseLists (HashTablePtr table);
HashTablePtr createHashTable(int size, int (*getKey)(void *), char *  (*toString)(void *), void (*freeHashObject)(void *), Boolean (compare)(void *, void *), int chain);
void HashInsert(HashTablePtr table, HashObjectPtr object);
HashObjectPtr HashSearch (HashTablePtr table, void * key);
void PrintHash(HashTablePtr table);
void FreeHashTable(HashTablePtr table);
HashObjectPtr HashRemove(HashTablePtr table, void * string);
int HashFunction(int key, int tableSize);
Boolean maxChainLengthReached(ListPtr h);
HashTablePtr resizeHashTable(HashTablePtr h);

#endif /* __HASHTABLE_H */ 
