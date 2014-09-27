/* 
 * File:   shell.h
 * Author: reuben
 *
 * Created on September 25, 2014, 7:08 PM
 */
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <regex.h>
#include "ourhdr.h"
#include "CommandParser.h"
#include "DirectoryTraverser.h"
#include "JobManager.h"

#ifndef SHELL_H
#define	SHELL_H
int handle_special_command(char**);
int handle_parent_commands(char**);

#endif	/* SHELL_H */

