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

/**
 * Handles any commmands that the parent is required to take care of
 * @param - the tokenized command
 * @return - nonzero if the parent handled the command passed
 */
int handle_parent_commands(char**);

#endif	/* SHELL_H */

#define VERSION_NUMBER "e91760b9f123bef1fb155189100510b8b56e8145"
