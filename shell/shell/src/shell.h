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

/**
 * Handles any commmands that the parent is required to take care of
 * @param - the tokenized command
 * @return - nonzero if the parent handled the command passed
 */
int handle_parent_commands(char**);

#endif	/* SHELL_H */

#define VERSION_NUMBER "5eeb7bb20ad7d47c7d763139dbde26c288e3a8ce"
