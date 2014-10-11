
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "Job.h"
#include "Node.h"
#include "List.h"


const int NUM_TESTS = 11;
/*const int DEBUG = 2;*/
const int DEBUG = 1;
int numNodes;

void print_stats(int *tests)
{
	printf(" ========================\n");
	printf(" Function  #invocations\n");
	printf(" ========================\n");
	printf(" addAtFront  %d \n", tests[0]);
	printf(" addAtRear   %d \n", tests[1]);
	printf(" removeFront %d \n", tests[2]);
	printf(" removeRear  %d \n", tests[3]);
	printf(" removeNode  %d \n", tests[4]);
	printf(" reverseList %d \n", tests[5]);
	printf(" search      %d \n", tests[6]);
	printf(" nullListandNodePass %d \n", tests[7]);
	printf(" addFirstNode %d \n", tests[8]);
	printf(" removeLastNode %d \n", tests[9]);
	printf(" badSearch    %d \n", tests[10]);
	printf(" ========================\n");
}

void sizeCheck(int listSize, ListPtr list, int testNumber) {
	if (listSize != list -> size)
		fprintf(stderr, "LIST SIZE DID NOT MATCH ON TEST #%d \n", testNumber);
}

void runRandomTests(int count, unsigned int seed, int n, ListPtr list) {
	int i;
	int test;
	NodePtr node;
	JobPtr job;
	int *tests;
	int listSize=numNodes;

	tests = (int *) malloc(sizeof(int) * NUM_TESTS);
	for (i = 0; i < NUM_TESTS; i++)
		tests[i] = 0;
	srandom(seed);
	for (i = 0; i < count; i++) {
		printf("\rRunning test #%d", i);
		test = (int) (NUM_TESTS * (double) rand() / RAND_MAX);
		tests[test]++;
		switch (test) {

		case 0:
			if (DEBUG > 1)
				fprintf(stderr, "addAtFront\n");
			n++;
			job = createJob(n, "some info");
			node = createNode(job);
			addAtFront(list, node);
			listSize++;
			sizeCheck(listSize, list, test);
			break;
		case 1:
			if (DEBUG > 1)
				fprintf(stderr, "addAtRear\n");
			n++;
			job = createJob(n, "some info");
			node = createNode(job);
			addAtRear(list, node);
			listSize++;
			sizeCheck(listSize, list, test);
			break;
		case 2:
			if (DEBUG > 1)
				fprintf(stderr, "removeFront\n");
			node = removeFront(list);
			if (node != NULL) listSize--;
			freeNode(node);
			sizeCheck(listSize, list, test);
			break;
		case 3:
			if (DEBUG > 1)
				fprintf(stderr, "removeRear\n");
			node = removeRear(list);
			if (node != NULL) listSize--;
			freeNode(node);
			sizeCheck(listSize, list, test);
			break;
		case 4:
			if (DEBUG > 1)
				fprintf(stderr, "removeNode\n");
			node = removeNode(list, search(list, i));
			if (node != NULL) listSize--;
			freeNode(node);
			sizeCheck(listSize, list, test);
			break;
		case 5:
			if (DEBUG > 1)
				fprintf(stderr, "reverseList\n");
			reverseList(list);
			break;
		case 6:
			if (DEBUG > 1)
				fprintf(stderr, "searchList\n");
			node = search(list, i);
			break;
		case 7:
			if (DEBUG > 1)
				fprintf(stderr, "nullListAndNodePass\n");
			ListPtr L = NULL;
			NodePtr node = NULL;
			freeList(L);
			getSize(L);
			isEmpty(L);
			addAtFront(L, node);
			addAtRear(L, node);
			freeNode(removeFront(L));
			freeNode(removeRear(L));
			freeNode(removeNode(L, node));
			reverseList(L);
			printList(L);
			break;
		case 8:
			if (DEBUG > 1)
				fprintf(stderr, "addFirstNode\n");

			ListPtr oneEleList = createList();
			NodePtr noder = createNode(createJob(0, "del"));
			addAtFront(oneEleList, noder);
			freeNode(removeFront(oneEleList));
			freeList(oneEleList);
			oneEleList = createList();
			addAtRear(oneEleList, noder);
			break;
		case 9:
			if (DEBUG > 1)
				fprintf(stderr, "removeLastNode\n");
			ListPtr oneEleList1 = createList();
			NodePtr noder1 = createNode(createJob(0, "del"));
			addAtFront(oneEleList1, noder1);
			freeNode(removeFront(oneEleList1));
			freeList(oneEleList1);

			oneEleList1 = createList();
			noder1 = createNode(createJob(0, "del"));
			addAtRear(oneEleList1, noder1);
			freeNode(removeRear(oneEleList1));

			oneEleList1 = createList();
			noder1 = createNode(createJob(0, "del"));
			addAtRear(oneEleList1, noder1);
			freeNode(removeNode(oneEleList1, noder1));
			break;
		case 10:
			if (DEBUG > 1)
				fprintf(stderr, "searchForNonExistentNode\n");
			search(list, 1000000);
			break;
		default:
			break;
		}
    }
	printf("\n");
	print_stats(tests);
	free(tests);
}

int main(int argc, char **argv)
{	
	int i;
	int n;
	int count;
	unsigned int seed=0;

	NodePtr node;
	JobPtr job;
	ListPtr list;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <list size> [<test size=list size>] [<seed>] \n",argv[0]);
		exit(1);
	}
	n = numNodes = atoi(argv[1]);
	count = n;
	if (argc >= 3) {
		count = atoi(argv[2]);
	}
	if (argc == 4) {
		seed = atoi(argv[3]);
	}

	list = createList();
	for (i=0; i<n; i++)
	{
		job = createJob(i, "args");
		node = createNode(job);
		/*printf("job ptr = %p node ptr = %p\n", job, node);*/
		addAtFront(list, node);
	}

	runRandomTests(count, seed, n, list);

	if (DEBUG > 0)
		printList(list);

	freeList(list);
	exit(0);
}

/* vim: set tabstop=4: */
