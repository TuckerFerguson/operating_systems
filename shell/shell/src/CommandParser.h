/* 
 * File:   CommandParser.h
 * Author: reubentanner
 *
 * Created on September 23, 2014, 7:51 PM
 */
#include <stdlib.h>
#include <string.h>

#ifndef __COMMANDPARSER_H
#define	__COMMANDPARSER_H

#define MAX_SHELL_ARGUMENTS  2049

char ** get_tokenized_command(char*);
        
#endif	/* COMMANDPARSER_H */

