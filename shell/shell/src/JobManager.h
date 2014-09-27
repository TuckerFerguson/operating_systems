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

#ifndef JOBMANAGER_H
#define	JOBMANAGER_H

Boolean is_background_job(char**);
Boolean start_background_job(char**);
void start_job(char*, char**);
void print_jobs();

#endif	/* JOBMANAGER_H */

