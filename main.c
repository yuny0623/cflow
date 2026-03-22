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
#include <stdlib.h>
#include "cli.h"
#include "scheduler.h" 
#include "dag.h"

int main() { 
	fprintf(stdout, "cflow start\n"); 
	
	// start CLI thread 
	pthread_t cli_thread; 
	int ret = pthread_create(&cli_thread, NULL, cli_input_receiver, NULL); 
	if (ret == -1) {
		perror("pthread_create"); 
		return 1; 
	}
	fprintf(stdout, "cli_thread started\n");


	struct dag_order* ord = calloc(1, sizeof(struct dag_order)); 
	ord = read_dag_dsl(); 
	if (ord == NULL) {
		fprintf(stderr, "read_dag_dsl has been failed\n");
		exit(EXIT_FAILURE); 
	}
	print_all_tasks(ord); 

	// start scheduler thread 
	pthread_t sched_thread; 
	ret = pthread_create(&sched_thread, NULL, sched_executor, ord); 
	if (ret == -1) {
		perror("pthread_create");
		return 1; 
	}
	fprintf(stdout, "sched_thread started\n"); 

	// Wait cli_thread to exit 
	pthread_join(cli_thread, NULL);
	pthread_join(sched_thread, NULL); 

	fprintf(stdout, "cflow exit\n"); 
	return 0; 
}

void print_all_tasks(struct dag_order* ord) { 
	int count = 0 ; 
	struct dag_order* p = ord; 
	while(p != NULL) {
		char* func_name = p->func_name; 
		fprintf(stdout, "------------------------ [%d] %s ------------------------ \n", count, func_name); 
		p = p->next; 
		count ++; 
	}
}