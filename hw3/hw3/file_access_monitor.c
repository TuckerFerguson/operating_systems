//
//  main.c
//  hw3
//
//  Created by Reuben Tanner on 11/3/14.
//  Copyright (c) 2014 Reuben Tanner. All rights reserved.
//

#include "file_acess_monitor.h"

static int get_pid();

monitor_ptr create_monitor(int max_number_of_processes)
{
    monitor_ptr monitor;
    monitor = (monitor_ptr)malloc(sizeof(struct file_access_monitor));
    monitor->max = max_number_of_processes;
    monitor->total = 0;
    pthread_cond_init(&(monitor->ok_to_access), NULL);
    pthread_mutex_init(&(monitor->lock), NULL);
    return monitor;
}

void access_file(monitor_ptr monitor)
{
    int pid = get_pid();
    if (!(pid > monitor->max))
    {
        pthread_mutex_lock(&(monitor->lock));
        
        while (monitor ->total+pid > monitor->max)
            pthread_cond_wait(&(monitor->ok_to_access), &(monitor->lock));
        
        fprintf(stderr, "Thread %d accessing file\n", pid);
        monitor ->total += pid;
        pthread_mutex_unlock(&(monitor->lock));
    }
}

void release_file(monitor_ptr monitor)
{
    pthread_mutex_lock(&(monitor->lock));
    
    pthread_cond_broadcast(&(monitor->ok_to_access));
    monitor->total -= get_pid();
    
    fprintf(stderr, "Thread %d releasing file\n", get_pid());
    pthread_mutex_unlock(&(monitor->lock));
}

static int get_pid()
{
    char name[16];
    pthread_getname_np(pthread_self(), name, 16);
    return atoi(name);
}
