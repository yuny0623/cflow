#include "cli.h"
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#define BUF_LEN 1024 // no semisolon after #define  

static void parse_input(char* buf);

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

		parse_input(buf); 
		fprintf(stdout, "input: %s\n", buf); 
	} 
}

static void parse_input(char* buf) { 

	return; 
}
