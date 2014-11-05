/**
 * CommandParser.h
 * 
 * This set of functions is useful for manipulating the lines entered by the user;
 * @param 
 * @return 
 */
#include <stdlib.h>
#include <string.h>
#include "common.h"

#ifndef __COMMANDPARSER_H
#define	__COMMANDPARSER_H

/**
 * The maximum amount of tokens accepted in a line of input to the shell
 */
#define MAX_SHELL_ARGUMENTS  2049

/**
 * Tokenizes the line of input provided to the shell.
 * @param - The line entered into the shell
 * @return - a 2d array of strings which represent the command tokenized
 */
char ** get_tokenized_command(char*);

/**
 * Determines if the line entered into the shell should be interpreted as a background command
 * @param - a 2d array of strings which represent the command toknized (the result from get_tokenized_command)
 * @return - the index of the token in which the ampersand is present
 */
int is_background_job(char**);

/**
 * Removes the ampersand from the token at the given index 2d array of strings
 * @param - The 2d array of strings representing the command tokenized
 * @param - the index at which the ampersand exists
 */
void remove_ampersand(char**, int);
        
#endif	/* COMMANDPARSER_H */

