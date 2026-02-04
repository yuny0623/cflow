#include "cli.h"
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#define BUF_LEN 1024 // no semisolon after #define  

void* cli_input_receiver(void *arg) {
        (void) arg; 

	char buf[BUF_LEN];
        read_again: 	
	memset(&buf, 0, sizeof(buf)); 
	int nbytes = read(STDIN_FILENO, buf, BUF_LEN - 1); // - 
	if (nbytes == -1) {
		if (errno == EINTR) {
			goto read_again; 	
		}
	}	

	fprintf(stdout, "input: %s\n", buf); 
}

void _parse_input(char* buf) { 
}
