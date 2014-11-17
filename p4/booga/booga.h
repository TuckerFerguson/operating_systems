
#ifndef __booga_H
#define __booga_H
/*
 * booga.h -- definitions for the char module
 */
#ifndef BOOGA_MAJOR
#define BOOGA_MAJOR 0   /* dynamic major by default */
#endif

#ifndef BOOGA_NUMBER_OF_MINORS
#define BOOGA_NUMBER_OF_MINORS 4    /* booga0 through booga3 */
#endif
/*
 * Split minors in two parts
 */
#define TYPE(dev)   (MINOR(dev) >> 4)  /* high nibble */
#define NUM(dev)    (MINOR(dev) & 0xf) /* low  nibble */

/*
 * The different configurable parameters
 */
struct booga_stats {
	long int bytes_read;
	long int bytes_written; 
	long int booga0_open; 
	long int booga1_open; 
	long int booga2_open; 
	long int booga3_open; 
	long int phrase0; 
	long int phrase1; 
	long int phrase2; 
	long int phrase3; 
};

#define READ_OPERATION 0
#define WRITE_OPERATION 1
#define PROCESS_KILLER 3

char** phrases= (char *[]){"booga! booga! ", "googoo! gaga! ", "wooga! wooga! ", "neka! maka! "};
const int PHRASE_COUNT = 4; 

typedef struct booga_stats booga_stats;

extern booga_stats booga_Device_Stats;

#endif /* __booga_H */
