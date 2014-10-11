//#include <stdio.h>
//#include <stdlib.h>
//
//#include "common.h"
//#include "Node.h"
//#include "List.h"
//#include "HashObject.h"
//#include "HashTable.h"
//
//const int NUM_TESTS = 5;
///*const int DEBUG = 2;*/
//const int DEBUG = 1;
//const int maxFileLine = 1024;
//
///**
//* print stats
//* params: the array of test counts
//* return: nothing
//* description: prints the amount of tests ran
//*/
//void print_stats(int *tests)
//{
//	printf(" ========================\n");
//	printf(" Function  #invocations\n");
//	printf(" ========================\n");
//	printf(" HashInsert  %d \n", tests[0]);
//	printf(" HashSearch   %d \n", tests[1]);
//	printf(" HashSearchBad %d \n", tests[2]);
//	printf(" HashInsertBad %d \n", tests[3]);
//	printf(" BadDataToAll %d \n", tests[4]);
////	printf(" HashRemove  %d \n", tests[3]);
////	printf(" HashRemoveOne  %d \n", tests[4]);
//	printf(" ========================\n");
//}
//
///**
//* runRandomTests
//* params: the number of tests to run, the seed value for the random generator, and the table to test
//* return: nothing
//* description: runs tests on stuff with things.
//*/
//void runRandomTests(int count, unsigned int seed, int n, HashTablePtr table)
//{
//	int i;
//	int test;
//	HashObjectPtr job;
//	int *tests;
//
//	char * inputString = (char *)malloc(sizeof(char) * strlen("I'm a void star") + 1);
//	inputString = strcpy(inputString, "I'm a void star");
//	char * firstInput = (char *)malloc(sizeof(char) * strlen("I'm a void star made at the beginning") + 1);
//	firstInput = strcpy (firstInput, "I'm a void star made at the beginning");
//	char * badString = (char *)malloc(sizeof(char) * strlen("I'm bad") + 1);
//	badString = strcpy (badString, "I'm bad");
//
//	tests = (int *) malloc(sizeof(int)*NUM_TESTS);
//	for (i=0; i<NUM_TESTS; i++)
//		tests[i]=0;
//   	srandom(seed);
//    for (i=0; i<count; i++) {
//		test = (int) (NUM_TESTS * (double) rand()/RAND_MAX);
//		tests[test]++;
//        switch (test) {
//            case 0:
//				if (DEBUG > 1) fprintf(stderr,"HashInsert\n");
//                n++;
//                job = createHashObject(inputString);
//                HashInsert(table, job);
//                break;
//            case 1:
//				if (DEBUG > 1) fprintf(stderr,"HashSearch\n");
//				char * XI = (char*)malloc(sizeof(char)*strlen("XI")+1);
//				XI = strcpy(XI, "xi");
//                job = createHashObject(XI); //found on line 3261
//
//                if (HashSearch(table, job->obj)==NULL)
//                	fprintf(stderr,"HashSearch failed\n");
//                free(XI);
//                break;
//            case 2:
//				if (DEBUG > 1) fprintf(stderr,"HashSearchBad\n");
//				job = createHashObject(badString);
//				if (HashSearch(table, job->obj))
//					fprintf(stderr,"Hash Search Bad Failed\n");
//                break;
//            case 3:
//				if (DEBUG > 1) fprintf(stderr,"HashInsertBad\n");
//				//job is uninitialized at this point
//				//HashInsert(table, job);
//                break;
//            case 4:
//				if (DEBUG > 1) fprintf(stderr,"BadDataToAll\n");
//				//HashInsert(table, (HashObjectPtr)"BADBAD");
//				//createHashObject(1214123);
//                break;
//            default:
//                break;
//        }
//    }
//    freeHashObject(job);
//    free(firstInput);
//    free(inputString);
//    free(badString);
//	print_stats(tests);
//	free(tests);
//}
//
///**
//* The driver
//* params:
//* return:
//* description:
//*/
//int main(int argc, char **argv)
//{
//	int n;
//	int count;
//	unsigned int seed=0;
//	FILE *file;
//	char line[maxFileLine];
//	char *token;
//	char *delims =  ",.;:\"&!? -_\n\t\r@()#$%^*+={}[]|<>/~`";
//	char *save;
//
//	HashObjectPtr job;
//	HashTablePtr table;
//
//	if (argc < 3) {
//		fprintf(stderr, "Usage: %s <table size> <file name> [<seed>] \n",argv[0]);
//		exit(1);
//	}
//	n = atoi(argv[1]);
//	count = n;
//	if (argc >=  3) {
//		file = fopen(argv[2], "rt");
//		//count = atoi(argv[2]);
//	}
//	if (argc == 4) {
//		seed = atoi(argv[3]);
//	}
//	char * firstInput = (char *)malloc(sizeof(char) * strlen("I'm a void star made at the beginning") + 1);
//	firstInput = strcpy (firstInput, "I'm a void star made at the beginning");
//	table = createHashTable(n, getKey, toString, freeHashObject, compare);
//	HashInsert(table, createHashObject(firstInput));
//
//	while(fgets(line, maxFileLine, file) != NULL)
//	{
//		save = (char*)(malloc((strlen(line)+1)*sizeof(char)));
//		strcpy (save, line);
//
//		if (strtok(save, delims) != NULL)
//		{
//			token = strtok(line, delims);
//			while (token != NULL)
//			{
//				token = strtok(NULL, delims);
//				if (token != NULL) {
//				job = createHashObject(token);
//				HashInsert(table, job);
//				}
//			}
//
//		}
//		free(save);
//	}
//	runRandomTests(count*1000, seed, n, table);
//	if (DEBUG > 0)
//		PrintHash(table);
//	free(firstInput);
//	FreeHashTable(table);
//	fclose(file);
//
//	exit(0);
//}
//
