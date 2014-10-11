/**
* File comment
* HashTable.c
* author: reuben
* description: this is a generic hash table file
*/
#include "HashTable.h"
#include "common.h"
#include "HashObject.h"
#include "List.h"
#include "Node.h"

/**
* createHashTable
* params: the size of the hash table, function pointers to functions specific to the type of data 
* being used in this assignment but still generic.
* return: a pointer to the hash table
* description: allocates memory for the hash table and all of its linked lists, sets the function pointers
* passed for the generic data to the function pointers used in this hash table.
*/
HashTablePtr createHashTable(int size, int (*getKey)(void *), 
							char * (*toString)(void *), 
							void (*freeHashObject)(void *), 
							Boolean (*compare)(void *, void *), 
							int maxChainLength)
{
	if (size == 0) return NULL;
	if (getKey == NULL) return NULL;
	if (toString == NULL) return NULL;
	if (freeHashObject == NULL) return NULL;
	if (compare == NULL) return NULL;

	HashTablePtr h = (HashTablePtr)malloc(sizeof(HashTable));
	h -> tableSize = size;
	h -> getKey = getKey;
	h -> toString = toString;
	h -> freeHashObject = freeHashObject;
	h -> compare = compare;
	h -> maxChainReached = FALSE;

	h -> table = malloc(sizeof(List)*size);
	int i;
	for (i = 0; i < size; i++)
	{
		h -> table[i] = createList(getKey, toString, freeHashObject, maxChainLength);
	}
	return h;
}

/**
* FreeHashTable
* params: the table to free
* return: oodles of memory
* description: frees the table
*/
void FreeHashTable(HashTablePtr table)
{
	if (table == NULL) return;
	if (table -> table == NULL) return;
	int i;
	for (i = 0; i < table -> tableSize; i++)
	{
		if (table -> table[i] == NULL) continue;
		freeList(table -> table[i]);
	}
	free(table -> table);
	free(table);
}

Boolean maxChainLengthReached(ListPtr l)
{
	if (l->maxChainLength == l->size)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




/**
* HashInsert
* params: the table to insert in and the hash object to insert.
* return: nothing
* description: validates the data passed, then inserts the hash object based on the 
* HashFunction of the key of its data object. This function also checks to ensure
* that the data is not currently in the table, if it is the frequency of the 
* existing piece of data is incremented.
*/
void HashInsert(HashTablePtr table, HashObjectPtr object)
{
	if (object == NULL) return;
	if (object -> obj == NULL) return;
	if (table == NULL) return;

	int key = HashFunction(getKey(object->obj), table -> tableSize);
	ListPtr l = table -> table[key];
	NodePtr n = search(l, object, compare);
	if (n == NULL)
	{
		object -> frequency++;
		addAtRear(l, createNode(object));
		if (maxChainLengthReached(l))
			table->maxChainReached = TRUE;
	}
	else
		((HashObjectPtr)(n -> data )) -> frequency++;
}

/**
* resizeHashTable
* params: the table to resize.
* return: a pointer to resized hashtable.
* description: Allocates memory for a hash table that
* is twice as big as the one passed in and then reinserts
* all current elements into the table. 
*/
HashTablePtr resizeHashTable(HashTablePtr h)
{
	if (h == NULL) return NULL;
	if (h -> table == NULL) return NULL;
	float multiplier = (h->tableSize/2)+2;

	HashTablePtr newTable = createHashTable(h->tableSize*multiplier, 
											getKey, 
											toString, 
											freeHashObject, 
											compare, 
											h->table[0]->maxChainLength);

	int i;
	for (i = 0; i < h -> tableSize; i++)
	{
		if (h -> table[i] == NULL) 
			continue;
		else
		{
			NodePtr temp = h->table[i]->head;
			while (temp->next != NULL)
			{
				HashObjectPtr hobj = temp->data;
				void * value = hobj->obj;
				HashObjectPtr newData = createHashObject(value);
				newData->frequency = hobj->frequency;				
				HashInsert(newTable, newData);
				temp = temp->next;
			}
		}
		
	}
	return newTable;
}


/**
* HashSearch
* params: the table to search in and the key to search for.
* return: a pointer to the hash object.
* description: determines the correct key for the hash object
* and then uses the linked list functions and the generic compare
* function to find the item. If it is not found, it returns a null value.
*/
HashObjectPtr HashSearch (HashTablePtr table, void * key)
{
	if (key == NULL) return NULL;
	if (table == NULL) return NULL;

	char* string = (char*)key;
	int theKey = HashFunction(getKey(string), table -> tableSize);
	HashObjectPtr h = createHashObject(string);
	NodePtr n = search(table -> table[theKey], h, compare);
	if (n == NULL)
		return NULL;
	else
		return (HashObjectPtr)n->data;
}

/**
* PrintHash
* params: the table to print
* return: nothing
* description: iterates through each linked list and uses the 
* list print.
*/
void PrintHash(HashTablePtr table)
{
	if (table == NULL) return;
	if (table -> table == NULL) return;
	int i;
	for (i = 0; i < table -> tableSize; i++)
	{
		if (table -> table[i] == NULL) continue;
		printList(table -> table[i]);
	}
}


/**
* HashRemove
* params: the table to remove from, the item to remove
* return: a pointer to the hash object that contained the item
* description: Removes an item from the hash table.
*/
HashObjectPtr HashRemove(HashTablePtr table, void * string)
{
	if (table == NULL) return NULL;
	if (table -> table == NULL) return NULL;
	HashObjectPtr lookFor = createHashObject(string);

	int i;
	for (i = 0; i < table -> tableSize; i++)
	{
		ListPtr list = table -> table[i];
		if (list == NULL || list->size == 0) continue;
		NodePtr node = removeNode(list, search(list, lookFor, compare));
		if (node != NULL)
			return node->data;
	}
	return NULL;
}

/**
* HashFunction
* params: the key to perform the hash function on, the table size to mod by
* return: the table key
* description: uses the division algorithm.
*/
int HashFunction(int key, int tableSize)
{
	if (tableSize == 0) return -1;

	return key % tableSize;
}
