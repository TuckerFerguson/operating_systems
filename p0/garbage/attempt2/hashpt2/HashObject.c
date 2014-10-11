/**
* File comment
* HashObject.c
* author: reuben
* description: this is a generic hash object file
*/
#include "HashObject.h"
#include "common.h"
#include <string.h>
#include <stdio.h>

/**
* createHashObject
* params: the data to create a hash object for
* return: a pointer to the hash object 
* description: this method creates a new hash object, any generic data can be passed into it if the function is then modified. 
*/
HashObjectPtr createHashObject(void * obj) 
{
	if (obj == NULL) return NULL;
	char * string = (char *)obj;
	HashObjectPtr h = (HashObjectPtr) malloc(sizeof(HashObject));
	h -> frequency = 0;
	h -> obj = (char *)malloc(sizeof(char) * (strlen(string) + 1));
	h -> obj = strcpy((char*)h->obj, string);
	return h;	
}

/**
* createHashObject
* params: the hash object to free
* return: nothing
* description: this method frees a hash object, any generic data can be passed into it if the function is then modified. 
*/
void freeHashObject(void * obj)
{
	if (obj == NULL) return;
	HashObjectPtr h = (HashObjectPtr)obj;
	if (h == NULL)
		return;
	free(h->obj);
	free(h);
}

/**
* toString
* params: generic data, in this case a hash object
* return: a string representing this piece of data
* description: a typical tostring method
*/
char * toString(void * obj)
{
	if (obj == NULL) return NULL;
	HashObjectPtr h = (HashObjectPtr)obj;
	if (h == NULL)
		return NULL;
	const int MAX_WORD_LENGTH = 100;
	char * theString = (char *)malloc(sizeof(char)*MAX_WORD_LENGTH + 1);
	sprintf(theString, "%s %d", (char *)h->obj, h->frequency);
	return theString;
}

/**
* getKey
* params: generic data, in this case a hash object to derive a key from 
* return: the key
* description: in this case, the key is derived from the sum of the ascii values 
* of the generic data, in this case a string, in the hash object.
*/
int getKey(void *obj)
{
	if (obj == NULL) return -1;
	char * theString = (char *)malloc(sizeof(char)*strlen((char*)obj) + 1);
	theString = strcpy(theString, (char*)obj);
	int length = strlen(theString);
	int i = 0, sum = 0;
	for (; i < length; i++)
	{
		char x = (char)(theString[i]);
		sum += x;
	}
	free(theString);
	return sum;
}

/**
* compare
* params: two generic pieces of data to compare, in this case HashObjects
* return: a true or false result
* description: compares two generic pieces of data, in this case HashObjects, if 
* the two hash objects have the same piece of generic data, in this case a string
* then the objects are equal.
*/
Boolean compare(void * obj1, void *obj2)
{
	if (obj1 == NULL || obj2 == NULL) return -1;
	HashObjectPtr h1 = (HashObjectPtr)obj1;
	HashObjectPtr h2 = (HashObjectPtr)obj2;
	if(h1 == NULL || h2 == NULL) return -1;

	char* h1obj = (char*)malloc(sizeof(char)*strlen((char*)h1->obj) +1);
	char* h2obj = (char*)malloc(sizeof(char)*strlen((char*)h2->obj) +1);
	h1obj = strcpy(h1obj, h1->obj);
	h2obj = strcpy(h2obj, h2->obj);

	if (!strcmp(h1obj, h2obj))
	{
		free(h1obj);
		free(h2obj);
		return TRUE;
	}
	free(h1obj);
	free(h2obj);
	return FALSE;
}

