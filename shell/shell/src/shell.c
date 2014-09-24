/* 
  lab/files-processes/simple-shell.c
  This program needs the file error.c and ourhdr.h to compile. 
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "List.h"
#include "Node.h"
#include "ourhdr.h"

int main(void)
{
	char *line;
	char *prompt = "dash>";
	pid_t	pid;
	int		status;

	using_history();
	while ((line=readline(prompt))) {
		printf("%s\n",line);
		add_history(line);

		if ( (pid = fork()) < 0)
			err_sys("fork error");

		else if (pid == 0) {		/* child */
			execlp(line, line, (char *) 0);
			err_ret("couldn't execute: %s", line);
			exit(EXIT_FAILURE);
		}

		/* parent */
		if ( (pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");
		printf("%% ");
		free(line);
	}
	exit(EXIT_SUCCESS);
}

/* vim: set ts=4: */
