//
//  webstats.h
//  webstats
//
//  Created by Reuben Tanner on 10/11/14.
//  Copyright (c) 2014 Reuben Tanner. All rights reserved.
//

/*
	Generate some web statistics  based on access logs from the Apache web server.
	In particular, this program looks for local versus total get requests.
	It also analyzes failed gets and total bytes transferred.
 
	The format of a Apache web server access log is as follows:
 
	For a successful get:
 
	1cust107.tnt5.phoenix.az.da.uu.net - - [13/Mar/2001:15:45:45 -0700]
 "GET /gas/gif/lib.gif HTTP/1.1" 200 1138
 
	The last field tells the number of bytes that were transferred. The second
	last field tells the status of the request: in this case 200 says it was
	successful. A value of 404 implies that the object requested was not
	found.
 
	For an unsuccessful get:
 
	j3009.inktomi.com - - [11/Mar/2001:04:07:31 -0700] "GET /~kani HTTP/1.0" 404 278
 
	Using the webstats program: The following show various ways of invoking the
	webstats  program.
 
 webstats access_log
 
 webstats access_log.1 access_log.2
 
 webstats access_log.*
 
	author: Amit Jain
 */

#ifndef webstats_webstats_h
#define webstats_webstats_h

#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ourhdr.h"

#define MEGABYTE 1048576
#define BYTES_DOWNLOADED_FIELD 9
#define HTTP_STATUS_CODE_FIELD 8
#define MAX_LINE_SIZE 4096
#define MAX_NUM_FIELDS 40

struct stats {
	double local_bytes;
	double total_bytes;
	double local_gets;
	double total_gets;
	double failed_gets;
	double local_failed_gets;
	char **url;
	int *requests;
	sem_t *lock;
} webstats;

long get_size_of_file(FILE*);
long get_number_of_threads();
int is_trivial_file(long file_size);
long get_chunk_size(long file_size, long num_threads);
void get_indices(FILE* fp, int chunk_number, long chunk_size, long *byte_endings, long *indices);
void process_chunk(FILE* file, int thread_index, pthread_t threads[], long begin_index, long end_index);
struct stats get_stats();
void return_stats();
/*
 * process_file(): The main function that processes one log file
 *
 *	input:
 * 		ptr: Pointer to log file name.
 *
 *	output:
 *		none
 *	return
 *		none
 */
void process_file(void*);

/*
 * 	print_webstats(): Print out webstats on standard output.
 *
 *	input:
 *		none
 *	output:
 *		none
 *
 *	return
 *		none
 */
static void print_webstats();
void* process(void *generic_file);

/*
 *	update_webstats(): Updates the webstats structure based on the input fields of current line.
 *
 *	input:
 *		num: The number of fields in the current webserver log entry
 *		field: An array of num strings representing the log entry
 *
 *	output:
 *		none
 */
static void update_webstats(char **field);
/*
 * initialize_webstats(): Initialize the webstats structure.
 *
 * 	input:
 * 		none
 *	output
 * 		none
 * 	return
 * 		none
 */

static void initialize_webstats();
/*
 * free_tokens(): Frees spaces allocated for tokens in parse_line
 *
 *	input:
 *	    field: an array of strings
 * 		num: An int representing number of tokens in array of strings named * 		field.
 *
 * 	output:
 *
 * 	return:
 * 		none.
 */
static void free_tokens(int num, char *field[]);
/*
 * parse_line(): Parse one line into string tokens separated by the given delimiters.
 * Beware: The input argument line is consumed in the process.
 *
 *	input:
 * 		line: A character array containing the current server log entry line
 * 		delim: A character array containing the delimiters to be used to separate fields in the line
 *
 * 	output:
 * 		field: An array of strings that represent the tokens found in the line.
 *
 * 	return:
 * 		num: The number of tokens found in the line.
 */

static int parse_line(char *line, const char *delim, char *field[]);

#endif
