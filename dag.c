/* ## DAG parse ## 
 * this component parse DAG dsl written in .dsl file. 
 * and generates order of DAG(Direct Asyclic Graph) and give it to scheduler
 *
 * ## call order ## 
 */

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h> // strlen, strstr, strdup 
#include<memory.h> // memcpy 
#include<stdlib.h> // <-- calloc 
#include"dag.h"

#define BUF_LEN 1024 
#define MAX_TASK_COUNT 10 
#define MAX_TASK_NAME_LEN 64       

void* handle_single_task(const char* dsl, struct dag_order* ord); 
void* handle_multiple_task(const char* dsl, struct dag_order* ord); 

struct dag_order* read_dag_dsl() {
	const char* dag_dsl_filename = "dag.dsl"; 
	int fd = open(dag_dsl_filename, O_RDONLY); 
	if (fd == -1){
		perror("open");
		return NULL; 
	}

	char buf[BUF_LEN]; 
	int nbytes = read(fd, &buf, BUF_LEN);
	if (nbytes == -1) {
		perror("read"); 
		return NULL; 
	}

	fprintf(stdout, "DSL read buffer: %s\n", buf); 

	struct dag_order* ord = parse_dag_order_dsl(buf); 
	if (ord == NULL) {
		fprintf(stderr, "Failed to parse dag order\n"); 
		return NULL; 
	}

	return ord; 
}

// trim back and forward 
// TODO("should be move to util file")
char* trim(const char* s) {
	char *ss = s; 
	size_t len = strlen(ss);
	// trim backward 
	for(int i = len - 1; i >= 0; i--) {
		if (ss[i] == ' ') {
			ss[i] = '\0'; 
		}
	}

	size_t lenlen = strlen(ss);
    char* new_start_pos = NULL; 	

	// trim forward 
	for(int i = 0; i < lenlen; i++) {
		if (ss[i] == ' ') {
			new_start_pos = ss[i]; 
		}
	}	

	return new_start_pos; 
}

// parse dsl (parse string like "A >> B >> C")
struct dag_order * parse_dag_order_dsl(const char* dsl) {
	fprintf(stdout, "parse dag order dsl start\n"); 
	struct dag_order* result_ord; 
	char* dsll = dsl; // could be like "A >> B >> C"
	
	char* pos = strstr(dsll, ">>");  // strstr -> finds the position of substr 
	int index = pos - dsl; 
	fprintf(stdout, "pos: %c, index: %d\n", pos, index); 

	if (pos == NULL) { 
		result_ord = calloc(1, sizeof(struct dag_order)); 
		handle_single_task(dsl, result_ord);		
	} else { 
		result_ord = calloc(1, sizeof(struct dag_order)); 
		handle_multiple_task(dsl, result_ord); 
	}

	fprintf(stdout, "Success to parse dag order dsl\n"); 
	return result_ord; 
}

void* handle_single_task(const char* dsl, struct dag_order* ord) { 
	fprintf(stdout, "handle_single_task called\n"); 
	if (strlen(dsl) <= 0) {
		fprintf(stderr, "no task definition in dsl: %s\n", dsl); 
		return NULL; 
	}

	// lets trim 
	char* func_name = trim(dsl); 
	ord->func_name = strdup(func_name); 
	ord->next = NULL; 
	ord->schedule = DAILY;  // first, use DAILY as default. 
}

void print_all_task(char tasks[][MAX_TASK_NAME_LEN], int count) { 
	for (int i = 0; i < count; i++) { 
		char* func_name = tasks[i]; 
		fprintf(stdout, "[print_all_tasks] func_name: %s\n", func_name); 
	}
}


// use strtok 
void* handle_multiple_task(const char* dsl, struct dag_order* ord) { 
	if (ord == NULL) {
		fprintf(stderr, "ord cannot be null\n");  
		return NULL; 
	}

	fprintf(stdout, "handle_multiple_task called\n"); 
	char tasks[MAX_TASK_COUNT][MAX_TASK_NAME_LEN]; 
	int count = 0; 
	char* token = strtok(dsl, ">");
	while (token != NULL && count < MAX_TASK_COUNT) {
		while (*token == ' ') { 
			token ++; 
		}

		char* end = token + strlen(token) - 1; 
		while (end > token && *end == ' ') { 
			*end = '\0';
			end--; 
		}

		strncpy(tasks[count], token, MAX_TASK_NAME_LEN - 1);
		tasks[count][MAX_TASK_NAME_LEN - 1] = '\0';

		count ++; 
		token = strtok(NULL, ">");
	}

	fprintf(stdout, "count: %d\n", count); 
	print_all_task(tasks, count); 
	print_all_task(tasks, count);
	print_all_task(tasks, count);

	if (count <= 0) { 
		return NULL; 
	}

	fprintf(stdout, "1\n"); 
	fflush(stdout); 
	struct dag_order* pointer = ord; 
	pointer->func_name = strdup(tasks[0]); 
	if (pointer->func_name == NULL) { 
		perror("strdup");
		return NULL; 
	}

	fprintf(stdout, "2\n");
	fflush(stdout); 
	pointer->next = NULL; 
	pointer->schedule = DAILY; // first, use DAILY as default 
 
	if (count <= 1) { 
		return NULL; 
	}

	for (int i = 1; i < count; i++) { 
		struct dag_order* next = calloc(1, sizeof(struct dag_order)); 
		pointer->next = next; 
		pointer = pointer->next; 
		
		pointer->func_name = strdup(tasks[i]);
		pointer->next = NULL; 
		pointer->schedule = DAILY; 
	}

	fprintf(stdout, "finish handle_multiple_task\n"); 
	fflush(stdout); 
	return NULL; 
}
