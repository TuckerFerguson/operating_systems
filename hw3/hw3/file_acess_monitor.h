//
//  file_acess_monitor.h
//  hw3
//
//  Created by Reuben Tanner on 11/3/14.
//  Copyright (c) 2014 Reuben Tanner. All rights reserved.
//

#ifndef hw3_file_acess_monitor_h
#define hw3_file_acess_monitor_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct file_access_monitor * monitor_ptr;

struct file_access_monitor {
    int max;
    int total;
    pthread_cond_t ok_to_access;
    pthread_mutex_t lock;
};

monitor_ptr create_monitor(int);
void access_file(monitor_ptr, int);
void release_file(monitor_ptr, int);

#endif
