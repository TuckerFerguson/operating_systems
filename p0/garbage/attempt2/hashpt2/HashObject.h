#ifndef __HASHOBJECT_H
#define __HASHOBJECT_H
#include <stdlib.h>
#include "common.h"

typedef struct hashobject HashObject;
typedef struct hashobject * HashObjectPtr;

struct hashobject {
	int frequency;
	void *obj; //going to be a char * for this assignment
};

HashObjectPtr createHashObject(void * obj);
void freeHashObject(void * obj);
char *toString(void * obj);
int getKey(void *obj);
Boolean compare(void * obj1, void *obj2);

#endif /* __HASHOBJECT_H */
