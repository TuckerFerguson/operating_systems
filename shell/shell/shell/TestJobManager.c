/*
 * TestJobManager.c
 *
 *      Author: reuben
 */

#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "ourhdr.h"
#include "JobManager.h"
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
    char* constString = "test&";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) >= 0);
            
    return TRUE;
}

Boolean notABackgroundJob1()
{
    char* constString = "test -lah";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) < 0);
            
    return TRUE;
}

Boolean notABackgroundJob2()
{
    char* constString = "test";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) < 0);
    return TRUE;
}

Boolean backgroundJobWith1ArgConjoined()
{
    char* constString = "test -lah&";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) >= 0);
    return TRUE;
}

Boolean backgroundJobWith1ArgSeparated()
{
    char* constString = "test -lah &";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) >= 0);
            
    return TRUE;
}

Boolean lotsOfArgumentsNotBackground()
{
    char* constString = "test -lah -a -d -s -p";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) < 0);
            
    return TRUE;
}

Boolean lotsOfArgumentsBackground()
{
    char* constString = "test -lah -a -d -s -p & ";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) >= 0);
            
    return TRUE;   
}

Boolean lotsOfArgumentsBackgroundConjoined()
{
    char* constString = "test -lah -a -d -s -p& ";
    char* conjoinedCommand = (char*)malloc(strlen(constString)+1 * sizeof(char));
    strcpy(conjoinedCommand, constString);
    char** command = get_tokenized_command(conjoinedCommand);
    
    myassert(is_background_job(command) >= 0);
            
    return TRUE;   
}

void runUnitTests()
{
	Boolean result;
	char *testName;

	testName = "conjoinedBackgroundJob";
	beforeTest(testName);
	result = conjoinedBackgroundJob();
	afterTest(testName, result);
        
        	testName = "notABackgroundJob1";
	beforeTest(testName);
	result = notABackgroundJob1();
	afterTest(testName, result);
        
        	testName = "notABackgroundJob2";
	beforeTest(testName);
	result = notABackgroundJob2();
	afterTest(testName, result);
        
        	testName = "backgroundJobWith1ArgConjoined";
	beforeTest(testName);
	result = backgroundJobWith1ArgConjoined();
	afterTest(testName, result);
        
        testName = "backgroundJobWith1ArgSeparated";
	beforeTest(testName);
	result = backgroundJobWith1ArgSeparated();
	afterTest(testName, result);
        
                testName = "lotsOfArugmentsBackgroundConjoined";
	beforeTest(testName);
	result = lotsOfArgumentsBackgroundConjoined();
	afterTest(testName, result);
        
                testName = "lotsOfArugmentsNotBackground";
	beforeTest(testName);
	result = lotsOfArgumentsNotBackground();
	afterTest(testName, result);
        
                testName = "lotsOfArugmentsBackground";
	beforeTest(testName);
	result = lotsOfArgumentsBackground();
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
