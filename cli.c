#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for EXIT_FAILURE 
#include <errno.h>
#include "cli.h"

#define BUF_LEN 1024 // no semisolon after #define  

int CLI_INPUT_ERROR = 0; 

static int parse_input(char* input_buf);

void* cli_input_receiver(void *arg) {
    (void) arg; 

	char input_buf[BUF_LEN];
    read_again: 	
	memset(&input_buf, 0, sizeof(input_buf)); 
	
	fprintf(stdout, "looping cli input receiver\n"); 
	while(1) { 
		errno = 0; 
		int nbytes = read(STDIN_FILENO, input_buf, BUF_LEN - 1); // receive command 
		if (nbytes == -1) {
			if (errno == EINTR) { // handle input error situation 
				goto read_again; 	
			} else {
				fprintf(STDIN_FILENO, "invalid input situation: %s\n", strerror(errno)); 
				CLI_INPUT_ERROR = 1; 
				break; 
			}
		}	

		char* command = parse_input(input_buf);
	    fprintf(stdout, "command: %s\n", command); 	
		fprintf(stdout, "input_fub: %s\n", input_buf); 
	} 

	if (CLI_INPUT_ERROR == 1) {
		fprintf(STDIN_FILENO, "Critical input error situation. exit this process\n"); 
		exit(EXIT_FAILURE); 	
	}
	
	return; 
}


void run_by_command(char* argv) {
   // TODO("")
}

/**
 * ## command ## 
 * 1. RUN: run existing dag 
 * 2. EXIT: exit this workflow 
 */ 
static int parse_input(char* input_buf) { 
	char argv[BUF_LEN]; 
	if (input_buf == "RUN") {
				// run dag 
	} else if (input_buf == "EXIT") { 
				// exit workflow 
	} else { // no suitable command
		return -1; 
	}

	fprintf(stdout, "parse input. input_buf: %s\n", input_buf); 
	return 0; 
}
