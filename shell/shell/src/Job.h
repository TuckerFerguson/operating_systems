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

JobPtr createJob (const int, const char *, const int);

char *toString(const void *);
void freeJob(const void *);
int compareTo(const void *,const void *);
void job_creation_printout(JobPtr);

#endif /* __JOB_H */
