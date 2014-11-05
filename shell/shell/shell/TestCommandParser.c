/*
 * TestCommandParser.c
 *
 *      Author: reuben
 */

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include "common.h"
#include "CommandParser.h"
    
/* 
 * macro to mimic the functionality of assert() from <assert.h>. The difference is that this version doesn't exit the program entirely.
 * It will just break out of the current function (or test case in this context).
 */
#define myassert(expr) if(!(expr)){ fprintf(stderr, "\t[assertion failed] %s: %s\n", __PRETTY_FUNCTION__, __STRING(expr)); return FALSE; }



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
	testCount++;
}

void afterTest(char* testName, Boolean result)
{
	printTestResult(testName, result);
	passCount += result;
}

Boolean conjoinedBackgroundJob()
{
    char* constString = "test -a";
    char* commandWithOneArg = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(commandWithOneArg, constString);
    char** command = get_tokenized_command(commandWithOneArg);
        
    myassert(!strcmp(command[0], "test"))
    myassert(!strcmp(command[1], "-a"))
            
    free(commandWithOneArg);
    int i;
    for (i = 0; i< 2049; i++)
        free(command[i]);
    free(command);
    return TRUE;
}

void runUnitTests()
{
	Boolean result;
	char *testName;

	testName = "simpleCommandWithOneArgParse";
	beforeTest(testName);
	result = conjoinedBackgroundJob();
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
