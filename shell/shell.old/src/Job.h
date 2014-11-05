#ifndef __JOB_H
#define __JOB_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define MAXPID_DIGITS 20
//exit status
#define RUNNING 1
#define COMPLETE 0
//job state
#define PROBLEM 1
#define NORMAL 0

typedef struct job  Job;
typedef struct job *JobPtr;

struct job {
    int pid;
    int job_id;
    char *original_command;
    int exit_status;
    int job_state;
};

/**
 * Creates a job
 * @param - the pid of the process
 * @param - the command used to create the process
 * @param - the job id
 * @return - the Job*
 */
JobPtr createJob (const int, const char *, const int);

/**
 * Prints a string representation of the Job
 * @param - the Job to print
 * @return - the string representing the Job
 */
char *toString(const void *);

/**
 * Frees the job
 * @param - the Job to free
 */
void freeJob(const void *);
/**
 * Determines if two Jobs are identical
 * @param - Job to compare
 * @param - Job to compare 
 * @return - nonzero if the Jobs are identical
 */
int compareTo(const void *,const void *);
void job_creation_printout(JobPtr);

#endif /* __JOB_H */
