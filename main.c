/* ## PROJECT ##
 * cflow 
 * a airflow like C written scheduling project. 
 * - starts at 2026.02.04(Web)-20:26:18 KST
 * - starts by razelo_dev(yuny0623)
 * - what for? just for fun.
 * - C/Linux Sys API
 */


/* ## DESIGN ## 
 * scheduler: handle task time management, and resource management 
 * task: a single unit of work, do actual things. 
 * context: holds little data between tasks (you can store useful small data)
 * paper: a function, that can loaded to task, (if you write paper(a function) it will runs on task)
 */


/* ## USE ## 
 * at first, just use it int CLI, no dashboard, no web console, just a CLI tooling 
 */

#include <stdio.h>
#include <pthread.h>
#include "cli.h"

int main() { 
	fprintf(stdout, "cflow start\n"); 
	
	pthread_t cli_thread; 
	int ret = pthread_create(&cli_thread, NULL, cli_input_receiver, NULL); 
	if (ret == -1) {
		perror("pthread_create"); 
		return 1; 
	}

	// Wait cli_thread to exit 
	pthread_join(cli_thread, NULL);
	fprintf(stdout, "cflow exit\n"); 
	return 0; 
}
