/* 
 * File:   DirectoryTraverser.h
 * Author: reuben
 *
 * Created on September 27, 2014, 1:10 PM
 */
//#include <List.h>
//#include <Node.h>
//#include "Object.h"
#include "errno.h"
#include "ourhdr.h"
#include "stdio.h"
#include "pwd.h"
#include "unistd.h"

#ifndef DIRECTORYTRAVERSER_H
#define	DIRECTORYTRAVERSER_H

#define MAX_DIRECTORY_SIZE 1024

/**
 * Gets the current directory
 * @return - the full path to the current working directory
 */
char* get_current_directory();
/**
 * Changes to the directory specified by the parameter
 * @param - the directory to change to
 */
void change_to_directory(char*);
/**
 * Changes to the users' home directory. If the directory cannot be determined, it will change to the root of the system.
 */
void change_to_home_directory();

#endif	/* DIRECTORYTRAVERSER_H */

