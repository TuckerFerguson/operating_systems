
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <windows.h>

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

#define MAX_LINE_SIZE 4096
#define MAX_NUM_FIELDS 40
#define MAX_THREADS 32

static void initialize_mutex();
static void lock_mutex();
static void unlock_mutex();

struct stats {
	double local_bytes;
	double total_bytes;
	double local_gets;
	double total_gets;
	double failed_gets;
	double local_failed_gets;
	char **url;
	int *requests;
	HANDLE mutex;
} webstats;

char *program_name;

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

static int parse_line(char *line, const char *delim, char *field[])
{
	char *next;
	int cnt = 0;
	char* save_ptr;

	//fprintf(stderr, "Value of line is %s\n", line);
	next = strtok_s(line, delim, &save_ptr);
	//fprintf(stderr, "Value of token is %s\n", next);
	while (next) {
		if (cnt == MAX_NUM_FIELDS - 1) break;
		field[cnt] = (char *)malloc(strlen(next) + 1);
		strcpy(field[cnt++], next);
		next = strtok_s(NULL, delim, &save_ptr);
	}
	field[cnt] = (char *)0; /* make the field array be null-terminated */

	return cnt;
}


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
static void free_tokens(int num, char *field[])
{
	int i;
	for (i = 0; i<num; i++)
		free(field[i]);
}



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

static void initialize_webstats(struct stats * stats)
{
	stats->total_bytes = 0;
	stats->total_gets = 0;
	stats->local_bytes = 0;
	stats->local_gets = 0;
	stats->failed_gets = 0;
	stats->local_failed_gets = 0;
}

static void initialize_mutex()
{
	webstats.mutex = CreateSemaphore(
		NULL,				// Security attributes
		0,					// Initial count (signaled)
		1,					// Maximum count (binary sem)
		(LPCWSTR)"ReaderSem"			// Name of sem
		);
}

static void lock_mutex()
{
	WaitForSingleObject(&webstats.mutex, INFINITE);
}

static void unlock_mutex()
{
	ReleaseSemaphore(&webstats.mutex, 1, NULL);
}


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
#define BYTES_DOWNLOADED_FIELD 9
#define HTTP_STATUS_CODE_FIELD 8
static void update_webstats(int num, char **field, struct stats * stats)
{
	int bytes_downloaded = atoi(field[BYTES_DOWNLOADED_FIELD]);

	stats->total_gets++;
	stats->total_bytes += bytes_downloaded;
	if (atoi(field[HTTP_STATUS_CODE_FIELD]) == 404) stats->failed_gets++;

	if ((strstr(field[0], "boisestate.edu") != NULL) || (strstr(field[0], "132.178") != NULL))
	{
		stats->local_gets++;
		stats->local_bytes += bytes_downloaded;
		if (atoi(field[HTTP_STATUS_CODE_FIELD]) == 404)
			stats->local_failed_gets++;
	}
}

static void combine_stats(struct stats * stats)
{
	lock_mutex();
	webstats.failed_gets += stats->failed_gets;
	webstats.local_bytes += stats->local_bytes;
	webstats.local_failed_gets += stats->local_failed_gets;
	webstats.local_gets += stats->local_gets;
	webstats.total_bytes += stats->total_bytes;
	webstats.total_gets += stats->total_gets;
	unlock_mutex();
}


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

static void print_webstats()
{
	printf("%10s %15s   %15s  %15s\n", "TYPE", "gets", "failed gets",
		"MB transferred");
	printf("%10s  %15.0f  %15.0f  %15.0f\n", "local", webstats.local_gets,
		webstats.local_failed_gets, (double)webstats.local_bytes / (1024 * 1024));
	printf("%10s  %15.0f  %15.0f  %15.0f\n", "total", webstats.total_gets,
		webstats.failed_gets, (double)webstats.total_bytes / (1024 * 1024));
}


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
DWORD WINAPI process(LPVOID resrvd)
{
	char *filename = (char *)resrvd;
	char *linebuffer = (char *)malloc(sizeof(char) * MAX_LINE_SIZE);
	char **field = (char **)malloc(sizeof(char *) * MAX_NUM_FIELDS);
	char *end_date = (char *)malloc(sizeof(char) * MAX_LINE_SIZE);

	struct stats * local_stats = (struct stats *)malloc(sizeof(struct stats));
	initialize_webstats(local_stats);

	FILE *fin = fopen(filename, "r");
	if (fin == NULL)
	{
		exit(1);
	}
	char *s = fgets(linebuffer, MAX_LINE_SIZE, fin);
	if (s != NULL)
	{
		int num = parse_line(linebuffer, " []\"", field);
		update_webstats(num, field, local_stats);
		printf("Starting date: %s\n", field[3]);
		free_tokens(num, field);

		while (fgets(linebuffer, MAX_LINE_SIZE, fin) != NULL)
		{
			int num = parse_line(linebuffer, " []\"", field);
			strcpy(end_date, field[3]);
			update_webstats(num, field, local_stats);
			free_tokens(num, field);
			strcpy(linebuffer, "");
		}
		printf("Ending date: %s\n", end_date);
		free(field);
		free(linebuffer);
		free(end_date);
		combine_stats(local_stats);
	}
	else{
		fprintf(stderr, "Line from file was null\n");
	}
	free(local_stats);
	return NULL;
}

void threaded_process_file(HANDLE *threads, DWORD *threadIds, int thread_index, char* file_name)
{
	HANDLE thread;
	DWORD threadId;
	thread = CreateThread(
		NULL,				// Security attributes
		0,					// Stack size (default)
		&process,			// Start function
		(LPVOID)file_name,				// Thread Parameter
		0,					// Creation flags
		&threadId		// Thread ID
		);

	if (thread == NULL){
		fprintf(stderr, "Unable to create thread. Exiting program\n");
		exit(1);
	}

	threads[thread_index] = thread;
	threadIds[thread_index] = threadId;
}

int main(int argc, char **argv)
{
	int i;

	if (argc <  2) {
		fprintf(stderr, "Usage: %s <access_log_file> {<access_log_file>} \n", argv[0]);
		exit(1);
	}

	program_name = (char *)malloc(strlen(argv[0]) + 1);
	strcpy(program_name, argv[0]);

	initialize_webstats(&webstats);
	initialize_mutex();

	HANDLE threads[MAX_THREADS];
	DWORD threadIds[MAX_THREADS];

	if (argc > 33) fprintf(stderr, "Number of files cannot exceed 32\n");

	for (i = 1; i<argc; i++)
	{
		threaded_process_file(threads, threadIds, i, argv[i]);
	}

	for (i = 1; i < argc; i++)
	{
		WaitForSingleObject(threads[i], INFINITE);
	}

	print_webstats();
	exit(0);

}

/* vim: set ts=4: */
