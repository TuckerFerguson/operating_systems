#include <stdio.h>
#include <stdlib.h>
#include "List.h"

/*

 list.c
 Contains functions to manipulate a doubly-linked list.
 
 */

/* private methods */

static NodePtr reverse(NodePtr L);
static void print(NodePtr node);

ListPtr createList() {
	ListPtr list;
	list = (ListPtr) malloc(sizeof(List));
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void freeList(ListPtr L) {
	if (L == NULL ) {
		return;
	}
	if (L->size == 0) {
		free(L);
		return;
	}
	if (L->size == 1 && L->head != NULL ) {
		freeNode(L->head);
		free(L);
		return;
	}
	if (L->size < 0 || (L->head != NULL && L->head->data == NULL) || (L->tail!= NULL && L->tail->data == NULL))
		return;

	NodePtr temp = L->head;

	while (temp->next != NULL ) {
		NodePtr tempPrev = temp;
		temp = temp->next;
		freeNode(tempPrev);
	}
	freeNode(temp);
	free(L);
	return;
}

int getSize(ListPtr L) {
	if (L == NULL )
		return 0;
	if (L->size < 0 || (L->head != NULL && L->head->data == NULL) || (L->tail!= NULL && L->tail->data == NULL))
		return 0;
	return L->size;
}

Boolean isEmpty(ListPtr L) {
	if (L == NULL )
		return TRUE;
	if (L->size == 0)
		return TRUE;
	else
		return FALSE;
}

void addAtFront(ListPtr list, NodePtr node) {
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
	if (list->head == NULL ) {
		list->head = node;
		list->tail = node;
	} else {
		list->head->prev = node;
		list->head = node;
	}
}

void addAtRear(ListPtr list, NodePtr node) {
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

NodePtr removeFront(ListPtr list) {
	if (list == NULL )
		return NULL ;
	if (list->head == NULL )
		return NULL ;
	if (list->tail == NULL )
		return NULL ;
	if (list->size <= 0)
		return NULL;
//	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
//		return NULL;
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

NodePtr removeRear(ListPtr list) {
	if (list == NULL )
		return NULL ;
	if (list->head == NULL )
		return NULL ;
	if (list->tail == NULL )
		return NULL ;
	if (list->size <= 0)
		return NULL;
//	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
//		return NULL;
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

NodePtr removeNode(ListPtr list, NodePtr node) {
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
//	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
//		return NULL;
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

NodePtr search(ListPtr list, int key) {
	if (list == NULL )
		return NULL ;
	if (list->size < 0 || (list->head != NULL && list->head->data == NULL) || (list->tail != NULL && list->tail->data == NULL))
		return NULL;
	if (key < 0)
		return NULL ;
	if (list ->size == 0 && list->head == NULL && list->tail ==NULL)
		return NULL;
	NodePtr temp = list->head;
	while (temp->next != NULL ) {
		if (temp->data->jobid == key)
			return temp;
		else
			temp = temp->next;
	}
	return NULL ;
}

void reverseList(ListPtr L) {
	if (L == NULL )
		return;
	if (L ->size == 0 && L->head == NULL && L->tail ==NULL)
		return;
	if (L->size < 0 || (L->head != NULL && L->head->data == NULL) || (L->tail != NULL && L->tail->data == NULL))
		return;
	L->tail = L->head;
	L->head = reverse(L->head);
}

static NodePtr reverse(NodePtr L) {
	if (L == NULL )
		return NULL ;

	NodePtr list = NULL;
	while (L != NULL ) {
		NodePtr tmp = L;
		L = L->next;
		if (L != NULL )
			L->prev = tmp;
		tmp->next = list;
		tmp->prev = L;
		list = tmp;
	}
	return list;
}

void printList(ListPtr L) {
	if (L == NULL )
		return;
	if (L->size < 0 || (L->head != NULL && L->head->data == NULL) || (L->tail != NULL && L->tail->data == NULL))
		return;
	if (L ->size == 0 && L->head == NULL && L->tail ==NULL)
		return;
	if (L)
		print(L->head);
}

static void print(NodePtr node) {
	if (node == NULL )
		return;
	int count = 0;
	char *output;

	while (node) {
		output = toString(node->data);
		printf(" %s -->", output);
		free(output);
		node = node->next;
		count++;
		if ((count % 6) == 0)
			printf("\n");
	}
	printf(" NULL \n");
}
