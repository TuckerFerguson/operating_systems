#ifndef __NODE_H
#define __NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef struct node Node;
typedef struct node * NodePtr;

struct node {
	void *data;
	NodePtr next;
	NodePtr prev;
};

NodePtr createNode (const void *data);
void freeNode(const NodePtr node, void (*freeObject)(const void *));

#endif /* __NODE_H */
