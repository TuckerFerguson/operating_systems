#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "Node.h"
#include "List.h"
#include "HashObject.h"
#include "HashTable.h"

HashTablePtr createNewHashTable();
void printOptions();
HashObjectPtr wordSearch();
FILE* uploadAndRunFile();
HashObjectPtr removeThings();

int MAX_INPUT_LENGTH = 1024;
HashTablePtr table = NULL;

void printOptions()
{
	printf("Please, select an option\n");
	printf("c - create a new hash table\n");
	printf("l - look up a word and its frequency\n");
	printf("f - read a specified file and update hash table\n");
	printf("p - print the current hash table\n");
	printf("r - remove a word\n");
	printf("q - quit\n\n");
}

int main(int argc, char **argv)
{	
	HashObjectPtr p;
	char * line = (char *)malloc(sizeof(char)*MAX_INPUT_LENGTH);

	printOptions();
	while (fgets(line, MAX_INPUT_LENGTH, stdin)!=NULL)
	{
		switch (line[0])
		{
			case 'c':
			if (table == NULL)
			{
				if ((table = createNewHashTable()) == NULL)
				{
					printOptions();
					break;
				}
				else
				{
					printf("\n\nSuccess. What would you like to do now?\n\n");
					printOptions();
				}
					
			}
			else 
			{
				FreeHashTable(table);
				if ((table = createNewHashTable()) == NULL)
				{
					printOptions();
					break;
				}
				else
				{
					printf("\n\nSuccess. What would you like to do now?\n\n");
					printOptions();
				}
			}
			break;
			case 'l':
				if (table == NULL)
				{
					printf("You need to first instantiate a hashtable, use the 'c' option\n\n");
					break;
				}
				else 
				{
					if ((p = wordSearch(table)) != NULL)
					{
						printf("\n\nSuccess. What would you like to do now?\n\n");
						printOptions();
					}
					else 
					{
						printOptions();
					}
				}

				break;
			case 'f':
				if (table == NULL)
				{
					printf("You need to first instantiate a hashtable, use the 'c' option\n\n");
					break;
				}
				if (uploadAndRunFile(table) != NULL)
				{
					printf("\n\nSuccess. What would you like to do now?\n\n");
					printOptions();
				}
				
				break;
			case 'p':
				if (table == NULL)
				{
					printf("You need to first instantiate a hashtable, use the 'c' option\n\n");
					break;
				}
				printf("\n\nPrinting Hash Table\n\n");
				PrintHash(table);
				printf("\n\nSuccess. What would you like to do now?\n\n");
				printOptions();
				break;
			case 'r':
				if (table == NULL)
				{
					printf("You need to first instantiate a hashtable, use the 'c' option\n\n");
					break;
				}
				removeThings();
				break;
			case 'q':
				FreeHashTable(table);
				free(line);
				printf("\n\nGoodbye\n\n");
				return 0;
			case 's':
				if (table == NULL)
				{
					printf("You need to first instantiate a hashtable, use the 'c' option\n\n");
					break;
				}

				break;
			default:
				printf("%s\n", "That is an unrecognized entry, please try again\n\n");
				printOptions();
				break;
		}
	}
	free(line);
	return 0;
}

HashTablePtr createNewHashTable()
{
	HashTablePtr h;
	int tableSize = 0;
	int chainSize = 0;
	char * input = (char *)malloc(sizeof(char)*MAX_INPUT_LENGTH);

	printf("Please enter the table size\n\n");
	fgets(input, MAX_INPUT_LENGTH, stdin);

	if ((tableSize = atoi(input)) != 0)
	{
		
	}
	else
	{
		printf("Please try again and enter an integer\n\n");
		return NULL;
	}
	printf("Please enter the chain length\n");
	fgets(input, MAX_INPUT_LENGTH, stdin);
	if ((chainSize = atoi(input)) != 0)
	{
		h = createHashTable(tableSize, getKey, toString, freeHashObject, compare, chainSize);
	}
	else 
	{
		printf("Please try again and enter an integer\n\n");
		free(input);
		return NULL;
	}
	free(input);
	return h;
}

HashObjectPtr wordSearch()
{
	char delims[] =  ",.;:\"&!? -_\n\t\r@()#$%^*+={}[]|<>/~`";
	char * input = (char *)malloc(sizeof(char)*MAX_INPUT_LENGTH);
	char * token;
	HashObjectPtr obj;

	printf("Please enter a word\n\n");
	fgets(input, MAX_INPUT_LENGTH, stdin);

	if (strtok(input, delims) != NULL)
	{
		token = strtok(input, delims);
	}
	if ((obj = HashSearch(table, token)) != NULL)
	{
		printf("%s %d\n", (char*)obj->obj, obj->frequency);
		return obj;
	}
	else
	{
		printf("%s\n", "The word was not found\n\n");
		return NULL;
	}
	free(input);
}

FILE* uploadAndRunFile()
{
	FILE *fp;
	char * input = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
	char * line = (char*)malloc(sizeof(char)*MAX_INPUT_LENGTH);
	char * token;
	HashObjectPtr extraJob;
	char delims[] =  ",.;:\"&!? -_\n\t\r@()#$%^*+={}[]|<>/~`";

	printf("%s\n", "Please enter the path to the file\n\n");
	fgets(input, MAX_INPUT_LENGTH, stdin);

	int j = 0;
	for (; j < strlen(input); j++)
	{
		if (input[j] == '\n')
		{
			input[j] = '\0';
			break;
		}
	}
	fp = fopen(input, "r+");

	if (fp != NULL)
	{
		while(!feof(fp))
		{
			fgets(line, MAX_INPUT_LENGTH, fp);
			if (strtok(line, delims) != NULL) 
			{
				token = strtok(line, delims);
				extraJob = createHashObject(token);
				HashInsert(table, extraJob);
				while (token != NULL) 
				{
				token = strtok(NULL, delims);
					if (token != NULL) 
					{
						token = strtok(NULL, delims);
						if (table->maxChainReached)
						{
							HashTablePtr oldTable = table;
							table = resizeHashTable(table);
							FreeHashTable(oldTable);
						}

							HashObjectPtr job = createHashObject(token);
							HashInsert(table, job);
					}


				}
			}
		}
		fclose(fp);
		free(input);
		free(line);
		return fp;
	}
	else
	{
		printf("%s\n", "That path was invalid, please try again\n\n");
		printOptions();
		free(input);
		free(line);
		return NULL;
	}

}

HashObjectPtr removeThings()
{
	char * input = (char *)malloc(sizeof(char)*MAX_INPUT_LENGTH);
	printf("Please enter the word you wish to remove\n\n");
	fgets(input, MAX_INPUT_LENGTH, stdin);

	int j = 0;
	for (; j < strlen(input); j++)
	{
		if (input[j] == '\n')
		{
			input[j] = '\0';
			break;
		}
	}
	HashObjectPtr hobj = HashRemove(table, input);
	if (hobj == NULL)
	{
		printf("That word was not found, returning to main menu\n\n");
		printOptions();
	}
	else
	{
		printf("That word has been successfully removed\n");
		printf("The word and frequency were %s\n", toString(hobj));
		printf("Please do something else\n\n");
		printOptions();
	}
	free(input);
	return hobj;
}
