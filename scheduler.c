/*
	## Scheduler ## 
	scheduler must run forever, 
*/

#include <stdio.h>
#include <time.h>
#include "scheduler.h"
#include "dag.h"
#include "task.h"

int single_execution_complete_flag = 1; 
char* execute_message; 
exit_flag = 0;

void* sched_executor(void* arg) {
	struct dag_order* ord = (struct dag_order*) arg; 
	dag_schedule sched = ord->schedule; 
	fprintf(stdout, "execute this schedule %d\n", sched); 

	struct dag_order* curr_ord = ord; 
	while(curr_ord != NULL) {
		char* func_name = curr_ord->func_name; 
		curr_ord = curr_ord->next; 
		int exec_result = execute_task(func_name);
		fprintf(stdout, "[%s] exec_result: %d\n", func_name, exec_result);  
	}

	fprintf(stdout, "exit sched_executor\n"); 
	return; 
}


// TODO
void exeute_hourly() {

}

// TODO 
void execute_daily() {

}
