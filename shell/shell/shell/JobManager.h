/* 
 * File:   JobManager.h
 * Author: reuben
 *
 * Created on September 27, 2014, 2:43 PM
 */
#include <common.h>
#include <List.h>
#include <Node.h>
#include "Job.h"
#include "ourhdr.h"
#include "unistd.h"
#include "CommandParser.h"
#include "wait.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "errno.h"

#ifndef JOBMANAGER_H
#define	JOBMANAGER_H

/**
 * Strips the ampersand from the command and execs it.
 * @param - the unmodified line entered into the shell 
 * @param - the tokenized command
 * @param - the index of the token containing the ampersand
 * @return - FALSE if the exec fails
 */
Boolean start_background_job(char*, char**, int);

/**
 * Begins a non-background job
 * @param - the unmodified line entered into the shell 
 * @param - the tokenized command
 * @return - FALSE if the exec fails
 */
Boolean start_job(char*, char**);

/**
 * Prints the jobs contained in the Job list
 */
void print_jobs();
/**
 * Initializes the Job list and the job id counter
 */
void init_job_manager();
/**
 * Adds a background job to the Job list for simple job management
 * @param - the pid of the process
 * @param - the line used to create the process
 */
void log_background_job(int, char*);
/**
 * Goes through the job list and determines the status of the processes and updates their state accorindgly
 */
void update_completed_jobs();
/**
 * Removes any completed jobs from the list, also appropriately resets the job id
 * @param 
 * @return 
 */
void remove_completed_jobs();
/**
 * Determines if the process is running or not
 * @param - the pid of the process
 * @return - nonzero if the process is still running
 */
int process_state(int);
/**
 * Determines if the process exited normally or otherwise
 * @param - the pid of the process
 * @return - nonzero if the process did not exit normally where normally is defined by exit(EXIT_SUCCESS)
 */
int exit_status(int);
/**
 * Frees jobs
 */
void free_jobs();


#endif	/* JOBMANAGER_H */

