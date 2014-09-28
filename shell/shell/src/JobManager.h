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

#ifndef JOBMANAGER_H
#define	JOBMANAGER_H

int is_background_job(char**);
Boolean start_background_job(char*, char**, int);
Boolean start_job(char*, char**);
void print_jobs();
void init_job_manager();
void log_background_job(int, char*);
void update_completed_jobs();

#endif	/* JOBMANAGER_H */

