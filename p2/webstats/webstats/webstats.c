#include "webstats.h"

char *program_name;
void print_field(char** field);
void validate_chunk(FILE* fp, char* validation_file_name, long begin_index, long end_index, long chunk_index);



int main(int argc, char **argv)
{
	int i;
	
	if (argc <  2) {
		fprintf(stderr,"Usage: %s <access_log_file> {<access_log_file>} \n",argv[0]);
		exit(1);
	}
	
	program_name = (char *) malloc(strlen(argv[0])+1);
	strcpy(program_name, argv[0]);
	
	initialize_webstats();
	
	for (i=1; i<argc; i++)
	{
		/*process the ith file*/
		process_file(argv[i]);
	}
	
	print_webstats();
	free(program_name);
	exit(0);
	
}

void process_file(void *ptr)
{
	char *filename = (char *) ptr;
	fprintf(stderr,"%s: processing log file %s\n", program_name, filename);
	
	FILE *file_to_process = fopen(filename,"r");
	if (file_to_process == NULL)
	{
		err_sys("Cannot open file %s\n", filename);
		exit(1);
	}
	
	long file_size = get_size_of_file(file_to_process);
//	if (is_trivial_file(file_size))
//	{
//		process((void*)file_to_process);
//	}
//	else
	{
		long num_threads = get_number_of_threads();
		long chunk_size = get_chunk_size(file_size, num_threads);
		pthread_t threads[num_threads];
		long byte_endings[num_threads];
		long indices[2];
		
		int i;
		for (i = 0; i < num_threads; ++i)
		{
			get_indices(file_to_process, i, chunk_size, byte_endings, indices);
//			validate_chunk(file_to_process, "validation.txt", indices[0], indices[1], i);
			process_chunk(file_to_process, i, threads, indices[0], indices[1]);
		}
		
		for (i = 0; i < num_threads; ++i)
			pthread_join(threads[i], NULL);
	}
}

void validate_chunk(FILE* fp, char* validation_file_name, long begin_index, long end_index, long chunk_index)
{
	long byte_size_of_chunk = end_index - begin_index;
	char* chunk = (char*)malloc(sizeof(char) * byte_size_of_chunk);
	fseek(fp, begin_index, SEEK_SET);
	fread((void*)chunk, sizeof(char), byte_size_of_chunk, fp);
	FILE* validation_file = fopen(validation_file_name,"a");
	fwrite(chunk, sizeof(char), strlen(chunk), validation_file);
}

void* process(void *file_chunk)
{
	char* chunk = (char*)file_chunk;
	char* string = (char*)malloc(sizeof(char)*strlen("tester1.txt")+1);
	
//	get_stats();
//	sprintf(string, "tester%f.txt", (i)webstats.local_bytes++);
//	return_stats();
//	
	FILE* file = tmpfile();
	if (file == NULL)
	{
		err_sys("Unable to create temp file\n");
		exit(EXIT_FAILURE);
	}
	fseek(file, 0L, SEEK_SET);
	fwrite(chunk, sizeof(char), strlen(chunk), file);
	rewind(file);
	
//	get_stats();
//	FILE* validation_file = fopen("validation.txt","a");
//	fwrite(chunk, sizeof(char), strlen(chunk), validation_file);
//	fclose(validation_file);
//	return_stats();
	
//	sleep(1);
//	printf("Thread id: %u\n", pthread_self());
//	printf("%s", chunk);

	
	char *linebuffer = (char *) malloc(sizeof(char) * MAX_LINE_SIZE);
	char **field = (char **) malloc(sizeof(char *) * MAX_NUM_FIELDS);
	char *end_date = (char *) malloc(sizeof(char) * MAX_LINE_SIZE);

//	char *s;
//	while((s= fgets(linebuffer, MAX_LINE_SIZE, file))!=NULL)
//		printf("%s", linebuffer);
	char *s = fgets(linebuffer, MAX_LINE_SIZE, file);
	if (s != NULL)
	{
//		printf("linebuffer\n");
//		printf("%s", linebuffer);
		int num = parse_line(linebuffer, " []\"", field);
//		printf("passed line parsing in thread %u\n", pthread_self());
		update_webstats(field);
		printf("Starting date: %s\n",field[3]);
		free_tokens(num, field);
		
		while (fgets(linebuffer, MAX_LINE_SIZE, file) != NULL)
		{
			int num = parse_line(linebuffer, " []\"", field);
//			print_field(field);
			strcpy(end_date, field[3]);
			update_webstats(field);
			free_tokens(num, field);
			strcpy(linebuffer,"");
		}
		printf("Ending date: %s\n", end_date);
//		free(end_date);
//		free(linebuffer);
//		free(field);
		fclose(file);
	}
	else
	{
		fprintf(stderr,"Unable to read chunk. Aborting thread\n");
		exit(EXIT_FAILURE);
	}
	printf("DONE!\n");
	return NULL;
}

void print_field(char** field)
{
	printf("Printing field\n");
	while (*(field)++)
		printf("%s ", *field);
	printf("\n");
}

void process_chunk(FILE* file, int thread_index, pthread_t threads[], long begin_index, long end_index)
{
	long byte_size_of_chunk = end_index - begin_index;
	char* chunk = (char*)malloc(sizeof(char) * byte_size_of_chunk);
	
	fseek(file, begin_index, SEEK_SET);
	fread((void*)chunk, sizeof(char), byte_size_of_chunk, file);

	pthread_t processing_thread;
	pthread_create(&processing_thread, NULL, process, (void*)chunk);
	threads[thread_index] = processing_thread;
}

static int parse_line(char *line, const char *delim, char *field[])
{
    char *next;
    int cnt=0;
    
    next = strtok(line, delim);
    while (next) {
        if (cnt == MAX_NUM_FIELDS-1) break;
        field[cnt] = (char *) malloc(strlen(next)+1);
        strcpy(field[cnt++],next);
        next =strtok(NULL, delim);
    }
    field[cnt] = (char *) 0; /* make the field array be null-terminated */
    
    return cnt;
}

void get_indices(FILE* fp, int chunk_number, long chunk_size, long *byte_endings, long *indices)
{
	long begin_index, end_index;
	if (chunk_number == 0)
	{
		begin_index = 0L;
		end_index = chunk_size;
	}
	else
	{
		begin_index = byte_endings[chunk_number-1];
		end_index = begin_index + chunk_size;
	}
	
	fseek(fp, end_index, SEEK_SET);
	char char_at_head;
	while((char_at_head = fgetc(fp)) != '\n' && char_at_head != EOF);
	end_index = ftell(fp);
	
	indices[0] = begin_index;
	indices[1] = end_index;
	byte_endings[chunk_number] = end_index;
	
#ifdef DEBUG
	printf("Chunk number: %d\n", chunk_number);
	printf("Final Begin index: %ld\n", begin_index);
	printf("Final End index: %ld\n", end_index);
#endif
}

struct stats get_stats()
{
//	sem_wait(webstats.lock);
//	return webstats;
	return webstats;
}

void return_stats()
{
//	sem_post(webstats.lock);
}

long get_size_of_file(FILE* fp)
{
	fseek(fp, 0L, SEEK_END);
	long size_of_file = ftell(fp);
	rewind(fp);
#ifdef DEBUG
	fprintf(stderr, "Size of file: %ld\n", size_of_file);
	fprintf(stderr, "Current position: %ld\n", ftell(fp));
#endif
	return size_of_file;
}

long get_chunk_size(long file_size, long num_threads)
{
#ifdef DEBUG
	fprintf(stderr, "Chunk size: %ld\n", (long)(file_size/num_threads));
#endif
	return file_size/num_threads;
}

long get_number_of_threads()
{
#ifdef DEBUG
	fprintf(stderr, "Number of threads: %ld\n", sysconf(_SC_NPROCESSORS_CONF));
#endif
	return sysconf(_SC_NPROCESSORS_CONF);
}

int is_trivial_file(long file_size)
{
#ifdef DEBUG
	fprintf(stderr, "File is trivial: %s\n", file_size < 5*MEGABYTE ? "TRUE" : "FALSE");
#endif
	if (file_size < 5*MEGABYTE)
		return 1;
	else
		return 0;
}

void free_tokens(int num, char *field[])
{
	int i;
	for (i=0; i<num; i++)
		free(field[i]);
}

void initialize_webstats()
{
	webstats.total_bytes = 0;
	webstats.total_gets = 0;
	webstats.local_bytes = 0;
	webstats.local_gets = 0;
	webstats.failed_gets = 0;
	webstats.local_failed_gets = 0;
	webstats.lock = sem_open("lock", O_CREAT);
}

void update_webstats(char **field)
{
	get_stats();
	int bytes_downloaded = atoi(field[BYTES_DOWNLOADED_FIELD]);
	
	webstats.total_gets++;
	webstats.total_bytes += bytes_downloaded;
	if (atoi(field[HTTP_STATUS_CODE_FIELD]) == 404) webstats.failed_gets++;
	
	if ((strstr(field[0], "boisestate.edu") != NULL) || (strstr(field[0], "132.178") != NULL))
	{
		webstats.local_gets++;
		webstats.local_bytes += bytes_downloaded;
		if (atoi(field[HTTP_STATUS_CODE_FIELD]) == 404)
			webstats.local_failed_gets++;
	}
	return_stats();
}

void print_webstats()
{
//	printf("Failed gets: %f\n", webstats.failed_gets);
//	printf("Local bytes: %f\n", webstats.local_bytes);
	printf("%10s %15s   %15s  %15s\n", "TYPE", "gets","failed gets",
		   "MB transferred");
	printf("%10s  %15.0f  %15.0f  %15.0f\n", "local", webstats.local_gets,
		   webstats.local_failed_gets, (double) webstats.local_bytes/(1024*1024));
	printf("%10s  %15.0f  %15.0f  %15.0f\n", "total", webstats.total_gets,
		   webstats.failed_gets, (double) webstats.total_bytes/(1024*1024));
}
/* vim: set ts=4: */
