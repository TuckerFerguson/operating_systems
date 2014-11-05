/**
* File comment
* Node.c
* author: reuben
* description: this is a generic linked list node
*/
#include "Node.h"

/**
* compare
* params: a generic piece of data
* return: a pointer to the generic node
* description: one of the more generic methods in this project,
* this will never have to be modified.
*/
NodePtr createNode(const void *obj)
{
	NodePtr newNode = (NodePtr) malloc (sizeof(Node));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->data = obj;
	return newNode;
}

/**
* freeNode
* params: the node to free and the free function of the generic data, whatever it may be.
* return: nothing
* description: takes the node to free and the free function of the generic data and uses them.
*/
void freeNode (const NodePtr node, void (*freeObject)(const void *))
{
	if (node == NULL) return;
	(*freeObject)(node->data);
	free(node);
}

