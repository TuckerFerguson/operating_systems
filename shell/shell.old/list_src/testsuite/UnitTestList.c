/*
 * UnitTestList.c
 *
 *      Author: reuben
 */

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "Object.h"
#include "Node.h"
#include "List.h"

/* 
 * macro to mimic the functionality of assert() from <assert.h>. The difference is that this version doesn't exit the program entirely.
 * It will just break out of the current function (or test case in this context).
 */
#define myassert(expr) if(!(expr)){ fprintf(stderr, "\t[assertion failed] %s: %s\n", __PRETTY_FUNCTION__, __STRING(expr)); return FALSE; }

ListPtr testlist;

int testCount = 0;
int passCount = 0;

void printTestInfo(char* testName, char *info)
{
	fprintf(stdout, "%s - %s\n", testName, info);
}

void printTestResult(char* testName, Boolean passed)
{
	if(passed == TRUE)
		fprintf(stdout, "%s - %s\n\n", "[PASSED]", testName);
	else
		fprintf(stdout, "%s - %s\n\n", "[FAILED]", testName);
}

void beforeTest(char* testName)
{
	printTestInfo(testName, "Running...");
	testlist = createList(compareTo, toString, freeObject);
	testCount++;
}

void afterTest(char* testName, Boolean result)
{
	printTestResult(testName, result);
	freeList(testlist);
	passCount += result;
}

NodePtr createTestNode(int jobid)
{
	ObjectPtr job = createObject(jobid, "cmd args");
	NodePtr node = createNode(job);
	return node;
}

Boolean addAtFrontWithNoNodes()
{
	NodePtr node = createTestNode(1);

	addAtFront(testlist, node);

	myassert(testlist->size == 1)
	myassert(testlist->head == node)
	myassert(testlist->tail == node)
	myassert(testlist->head->next == NULL)
	myassert(testlist->head->prev == NULL)

	return TRUE;
}

Boolean addAtFrontWithOneNode()
{
//	printTestInfo("addAtFrontWithOneNode", "(not implemented)");
    
    NodePtr node1 = createTestNode(1);
    addAtFront(testlist, node1);

    NodePtr node2 = createTestNode(2);
    addAtFront(testlist, node2);
    
    myassert(testlist->size == 2)
	myassert(testlist->head == node2)
	myassert(testlist->tail == node1)
	myassert(testlist->head->next == node1)
	myassert(testlist->head->prev == NULL)
    myassert(testlist->tail->next == NULL)
	myassert(testlist->tail->prev == node2)
    
	return TRUE;
}

Boolean addAtRearWithNoNodes()
{
	NodePtr node = createTestNode(1);
    
	addAtRear(testlist, node);
    
	myassert(testlist->size == 1)
	myassert(testlist->head == node)
	myassert(testlist->tail == node)
	myassert(testlist->head->next == NULL)
	myassert(testlist->head->prev == NULL)
    
	return TRUE;
}

Boolean addAtRearWithOneNode()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node2);
    
    myassert(testlist->size == 2)
	myassert(testlist->head == node1)
	myassert(testlist->tail == node2)
	myassert(testlist->head->next == node2)
	myassert(testlist->head->prev == NULL)
    myassert(testlist->tail->next == NULL)
	myassert(testlist->tail->prev == node1)
    
	return TRUE;
}

Boolean removeFromListWithOneNode()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    removeFront(testlist);
    
    myassert(testlist->size == 0)
	myassert(testlist->head == NULL)
	myassert(testlist->tail == NULL)
    
    NodePtr node2 = createTestNode(2);
    addAtFront(testlist, node2);
    removeRear(testlist);
    
    myassert(testlist->size == 0)
	myassert(testlist->head == NULL)
	myassert(testlist->tail == NULL)
    
	return TRUE;
}

Boolean addNullNode()
{
    addAtRear(testlist, NULL);
    addAtFront(testlist, NULL);
    removeFront(testlist);
    removeRear(testlist);
    search(testlist, NULL);
    removeNode(testlist, NULL);
    
    myassert(testlist->size == 0)
	myassert(testlist->head == NULL)
	myassert(testlist->tail == NULL)
    return TRUE;
}

Boolean nullNodeInTheMiddle()
{
    NodePtr node1 = createTestNode(1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node1);
    addAtRear(testlist, NULL);
    addAtRear(testlist, node2);
    myassert(testlist->size == 2)
	myassert(testlist->head == node1)
	myassert(testlist->tail == node2)
    return TRUE;
}

Boolean searchTest()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node2);
    NodePtr node3 = createTestNode(3);
    addAtRear(testlist, node3);
    
    int data = 3;
    NodePtr res = search(testlist, (void*)&data);
    
    if (res)
        return TRUE;
    else
        return FALSE;
}

Boolean search_ExpectFind_Test()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node2);
    NodePtr node3 = createTestNode(3);
    addAtRear(testlist, node3);
    
    int data = 3;
    NodePtr res = search(testlist, (void*)&data);
    
    if (res)
        return TRUE;
    else
        return FALSE;
}

Boolean search_ExpectNotFind_Test()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node2);
    NodePtr node3 = createTestNode(3);
    addAtRear(testlist, node3);
    
    int data = 4;
    NodePtr res = search(testlist, (void*)&data);
    
    if (res)
        return FALSE;
    else
        return TRUE;
}

Boolean search_ExpectFindWithNull_Test()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node2);
    addAtRear(testlist, NULL);
    NodePtr node3 = createTestNode(3);
    addAtRear(testlist, node3);
    
    int data = 3;
    NodePtr res = search(testlist, (void*)&data);
    
    if (res)
        return TRUE;
    else
        return FALSE;
}

Boolean reverseListTest()
{
    NodePtr node1 = createTestNode(1);
    addAtRear(testlist, node1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node2);
    NodePtr node3 = createTestNode(3);
    addAtRear(testlist, node3);
    
    reverseList(testlist);
    
    myassert(testlist->size == 3)
	myassert(testlist->head == node3)
	myassert(testlist->tail == node1)
	myassert(testlist->head->next == node2)
	myassert(testlist->head->prev == NULL)
    myassert(testlist->tail->next == NULL)
	myassert(testlist->tail->prev == node2)
    
    return TRUE;
}

Boolean removeNodeTest()
{
    NodePtr node1 = createTestNode(1);
    NodePtr node2 = createTestNode(2);
    addAtRear(testlist, node1);
    addAtRear(testlist, NULL);
    addAtRear(testlist, node2);
    
    removeNode(testlist, node2);
    
    myassert(testlist->size == 1)
	myassert(testlist->head == node1)
	myassert(testlist->tail == node1)
    return TRUE;
}

void runUnitTests()
{
	Boolean result;
	char *testName;

	testName = "addAtFrontWithNoNodes";
	beforeTest(testName);
	result = addAtFrontWithNoNodes();
	afterTest(testName, result);

	testName = "addAtFrontWithOneNode";
	beforeTest(testName);
	result = addAtFrontWithOneNode();
	afterTest(testName, result);

	testName = "addAtRearWithNoNodes";
	beforeTest(testName);
	result = addAtRearWithNoNodes();
	afterTest(testName, result);

	testName = "addAtRearWithOneNode";
	beforeTest(testName);
	result = addAtRearWithOneNode();
	afterTest(testName, result);

	testName = "removeFromListWithOneNode";
	beforeTest(testName);
	result = removeFromListWithOneNode();
	afterTest(testName, result);
    
    testName = "search_ExpectFind_Test";
	beforeTest(testName);
	result = search_ExpectFind_Test();
	afterTest(testName, result);
    
    testName = "search_ExpectNotFind_Test";
	beforeTest(testName);
	result = search_ExpectNotFind_Test();
	afterTest(testName, result);
    
    testName = "reverseList";
	beforeTest(testName);
	result = reverseListTest();
	afterTest(testName, result);

    testName = "addNullNode";
	beforeTest(testName);
	result = addNullNode();
	afterTest(testName, result);
    
    testName = "nullNodeInTheMiddle";
	beforeTest(testName);
	result = nullNodeInTheMiddle();
	afterTest(testName, result);

    testName = "search_ExpectFindWithNull_Test";
	beforeTest(testName);
	result = search_ExpectFindWithNull_Test();
	afterTest(testName, result);
    
    testName = "removeNodeTest";
	beforeTest(testName);
	result = removeNodeTest();
	afterTest(testName, result);

	fprintf(stdout, "Test Cases: %d\n",  testCount);
	fprintf(stdout, "Passed: %d\n", passCount);
	fprintf(stdout, "Failed: %d\n", testCount - passCount);
}

int main(int argc, char *argv[])
{
	runUnitTests();
	exit(0);
}
