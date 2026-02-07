#include "cli.h"
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#define BUF_LEN 1024 // no semisolon after #define  

static char* parse_input(char* buf);

void* cli_input_receiver(void *arg) {
        (void) arg; 

	char buf[BUF_LEN];
        read_again: 	
	memset(&buf, 0, sizeof(buf)); 
	
	fprintf(stdout, "looping cli input receiver\n"); 
	while(1) { 
		int nbytes = read(STDIN_FILENO, buf, BUF_LEN - 1); 
		if (nbytes == -1) {
			if (errno == EINTR) {
				goto read_again; 	
			}
		}	

		char* argv = parse_input(buf);
	       	fprintf(stdout, "argv: %s\n", argv); 	
		fprintf(stdout, "input: %s\n", buf); 
	} 
}


void run_by_command(char* argv) {
   // TODO("")
}

/**
 * ## command ## 
 * 1. run dag 
 * 2. exit dag 
 */ 
static char* parse_input(char* buf) { 
	char argv[BUF_LEN]; 
	char* save; 
	char* tok = strtok_r(buf, " ", &save); // strtok_r is thread-safe, so use this instead of strtok() 
	
	int idx = 0; 
	while(tok) {
		argv[idx++] = tok; 
		tok = strtok_r(NULL, " ", &save); 
	}

	fprintf(stdout, "parse input finish\n"); 
	return argv;  
}
