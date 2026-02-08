/*
	## Scheduler ## 
	scheduler must run forever, 
*/

#include <stdio.h>
#include "scheduler.h"
#include "dag.h"
#include "task.h"
#include <time.h>

int single_execution_complete_flag = 1; 
char* execute_message; 

void* sched_executor(void* arg) {
	(struct dag_order*) arg; 
	
	// manage workflow forever 
	
	dag_schedule sched = arg->schedule; 
	fprintf(stdout, "execute this schedule %d\n", sched); 

	char* func_name = arg->func_name; 
	struct dag_order* next_dag_order = arg->next; 
	while(exit_flag) {
		fprintf(stdout, "execute func_name: %s\n", func_name); 
		task_func_table.find_func();
		next_dag_order = next_dag_order->next; 
		if (next_dag_order == NULL) {
			fprintf(stdout, "func_execution has been finished\n"); 
			single_execution_complete_flag = 0; 	
			execute_message = "next_dag_order is NULL"; 
		}
		func_name = next_dag_order->func_name; 	
	}

	fprintf(stdout, "exit sched_executor\n"); 
	return; 
}


void exeute_hourly() {

}

void execute_daily() {

}
