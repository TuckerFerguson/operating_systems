#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <regex.h>
#include "ourhdr.h"
#include "CommandParser.h"
#include "DirectoryTraverser.h"
#include "JobManager.h"
#include <List.h>

#ifndef SHELL_H
#define	SHELL_H

#define MAX_PIPE_LENGTH 4096
#define FAIL "FAIL"

/**
 * Handles any commmands that the parent is required to take care of
 * @param - the tokenized command
 * @return - nonzero if the parent handled the command passed
 */
int handle_parent_commands(char**);

#endif	/* SHELL_H */

#define VERSION_NUMBER "0a2d33eb0f08eba0986fd328d5c1763e95f136d2"
