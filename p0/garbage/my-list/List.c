/**
* File comment
* List.c
* author: reuben
* description: this is a generic linked list file
*/

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Node.h"
#include "common.h"

/*

	list.c 
		Contains functions to manipulate a doubly-linked list.
 
*/


/* private methods */

static NodePtr reverse(NodePtr);
static void print(NodePtr node, char * (*toString)(void *));


/**
 * This method creates an empty list with size 0 and no head and tail pointer
 */
ListPtr createList(int(*compareTo)(const void *, const void *),
                   char * (*toString)(const void *),
				   void (*freeHashObject)(const void *))
{
	ListPtr list;
	list = (ListPtr) malloc(sizeof(List));
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->compareTo = compareTo;
	list->toString = toString;
	list->freeObject = freeHashObject;
	return list;
}

/**
 * This method is the deconstructor for the List class
 */
void freeList(ListPtr list) {
	if (list == NULL ) {
		return;
	}
	if (list->size == 0) {
		free(list);
		return;
	}
	if (list->size == 1 && list->head != NULL ) {
		freeNode(list->head, list->freeObject);
		free(list);
		return;
	}
	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail!= NULL && list->tail->data == NULL))
		return;

	NodePtr temp = list->head;

	while (temp->next != NULL ) {
		NodePtr tempPrev = temp;
		temp = temp->next;
		freeNode(tempPrev, list->freeObject);
	}
	freeNode(temp, list->freeObject);
	free(list);
	return;
}

/**
 * This method returns the size of the List passed in
 */
int getSize(ListPtr list)
{
	if (list == NULL)
		return 0;
	return list->size;
}

/**
 * This method returns whether or not the list is empty
 */
Boolean isEmpty(ListPtr list)
{
	if (list == NULL )
		return 0;
	if (list->size == 0)
		return TRUE;
	else
		return FALSE;
}

/**
 * This method adds the passed in node at the front of the list, making it the new head
 */
void addAtFront(ListPtr list, NodePtr node)
{
	if (list == NULL )
		return;
	if (node == NULL )
		return;
	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
		return;
	if (list->size == 0 && (list->tail == NULL && list->head == NULL )) {
		node->next = NULL;
		node->prev = NULL;
		list->head = node;
		list->tail = node;
		list->size++;
		return;
	}
	list->size++;
	node->next = list->head;
	node->prev = NULL;
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	} else {
		list->head->prev = node;
		list->head = node;
	}
}

/**
 * This method adds the passed in node at the tail
 */
void addAtRear(ListPtr list, NodePtr node)
{
		if (list == NULL )
		return;
	if (node == NULL )
		return;
	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
		return;
	if (list->size == 0 && (list->tail == NULL && list->head == NULL )) {
		node->next = NULL;
		node->prev = NULL;
		list->head = node;
		list->tail = node;
		list->size++;
		return;
	}
	node->next = NULL;
	node->prev = NULL;
	list->size++;

	NodePtr temp = list->head;
	while (temp->next != NULL ) {
		temp = temp->next;
	}
	temp->next = node;
	node->prev = temp;
	list->tail = node;
}

/**
 * This method removes the head of the list
 */
NodePtr removeFront(ListPtr list)
{
	if (list == NULL )
		return NULL ;
	if (list->head == NULL )
		return NULL ;
	if (list->tail == NULL )
		return NULL ;
	if (list->size <= 0)
		return NULL;
	if (list ->size == 0 && list->head == NULL && list->tail ==NULL)
		return NULL;
	if (list->size == 1 && list->head != NULL && list->tail != NULL ) {
		NodePtr temp = list->head;
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return temp;
	}
	NodePtr temp = list->head;
	list->head = list->head->next;
	list->head->prev = NULL;
	temp->next = NULL;
	list->size--;
	return temp;
}

/**
 * This method removes the tail of the list
 */
NodePtr removeRear(ListPtr list)
{
	if (list == NULL )
		return NULL ;
	if (list->head == NULL )
		return NULL ;
	if (list->tail == NULL )
		return NULL ;
	if (list->size <= 0)
		return NULL;
	if (list ->size == 0 && list->head == NULL && list->tail ==NULL)
		return NULL;
	if (list->size == 1 && list->head != NULL && list->tail != NULL ) {
		NodePtr temp = list->head;
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return temp;
	}
	NodePtr temp = list->head;
	while (temp->next != NULL )
		temp = temp->next;
	list->tail = temp->prev;
	temp->prev = NULL;
	list->tail->next = NULL;
	list->size--;
	return temp;
}

/**
 * This method removes the node whose pointer is passed in
 */
NodePtr removeNode(ListPtr list, NodePtr node)
{
	if (list == NULL )
		return NULL ;
	if (list->head == NULL )
		return NULL ;
	if (list->tail == NULL )
		return NULL ;
	if (node == NULL )
		return NULL ;
	if (list -> size <= 0 )
		return NULL;
	if (list ->size == 0 && list->head == NULL && list->tail ==NULL)
		return NULL;
	if (node == list->head || node->prev == NULL )
		return removeFront(list);

	if (node == list->tail || node->next == NULL )
		return removeRear(list);

	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->next = NULL;
	node->prev = NULL;
	list->size--;
	return node;
}

/**
 * this method searches the nodes in the list to find one with the same jobid as the passed in value
 */
NodePtr search(const ListPtr list, const void * searchFor)
{
	if (list == NULL )
		return NULL ;
	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
		return NULL;
	if (list ->size == 0 && list->head == NULL && list->tail ==NULL)
		return NULL;
	if (searchFor == NULL)
		return NULL;

	NodePtr temp = list->head;
	while (temp != NULL ) {
		if (list->compareTo(searchFor, temp->data))
			return temp;
		else
			temp = temp->next;
	}
	return NULL;
}

/**
 * This method reverses the list.
 */
void reverseList(ListPtr L)
{
	if (L == NULL )
		return;
	if (L ->size == 0 && L->head == NULL && L->tail ==NULL)
		return;
	if (L->size < 0 || (L->head != NULL && L->head->data == NULL) || (L->tail != NULL && L->tail->data == NULL))
		return;
	L->tail = L->head;
	L->head  = reverse (L->head);
}

/**
 * A helper method for the reverse List method
 */
static NodePtr reverse(NodePtr L)
{
	if (L == NULL )
		return NULL ;
	NodePtr list = NULL;
	while (L != NULL) {
		NodePtr tmp = L;
		L = L->next;
		if (L != NULL) L->prev = tmp;
		tmp->next = list;
		tmp->prev = L;
		list = tmp;
	}
	return list;
}

/**
 * This method prints the list passed into it
 */
void printList(ListPtr L)
{
	if (L == NULL )
		return;
	if (L->size < 0 || (L->head != NULL && L->head->data == NULL) || (L->tail != NULL && L->tail->data == NULL))
		return;
	if (L ->size == 0 && L->head == NULL && L->tail ==NULL)
		return;
	if (L) print(L->head, L->toString);
}

/**
 * This method prints the node passed into it
 */
static void print(NodePtr node, char * (*toString)(void *))
{
	if (node == NULL )
		return;
	int count = 0;
	char *output;

	while (node) {
		output = (*toString)(node->data);
		printf("%s\n", output);
	    free(output);
		node = node->next;
		count++;
	}
}
 

		

