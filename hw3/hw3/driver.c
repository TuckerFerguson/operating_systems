//
//  driver.c
//  hw3
//
//  Created by Reuben Tanner on 11/3/14.
//  Copyright (c) 2014 Reuben Tanner. All rights reserved.
//

#include "file_acess_monitor.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_THREADS 16

void* operate_on_file(void * arg);
static void set_thread_name(int id);

int n;
monitor_ptr monitor;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: ./driver <n>\n");
        exit(EXIT_FAILURE);
    }
    
    n = atoi(argv[1]);
    pthread_t threads[MAX_THREADS];
    
    monitor = create_monitor(n);
   
    int i;
    for (i = 0; i < MAX_THREADS; ++i)
    {
        pthread_create(&(threads[i]), NULL, operate_on_file, (void*)(rand() % monitor->max));
    }
    for (i = 0; i < MAX_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    
}

void* operate_on_file(void * arg)
{
    set_thread_name((int)arg);
    access_file(monitor);
    sleep(1);
    release_file(monitor);
    
    return NULL;
}

static void set_thread_name(int id)
{
    char name[16];
    sprintf(name, "%d", id);
    pthread_setname_np(name);
}