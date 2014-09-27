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

char* get_current_directory();
void change_to_directory(char*);
void change_to_home_directory();

#endif	/* DIRECTORYTRAVERSER_H */

