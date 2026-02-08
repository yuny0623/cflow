/* ## DAG parse ## 
 * this component parse DAG dsl written in .dsl file. 
 * and generates order of DAG(Direct Asyclic Graph) and give it to scheduler
 *
 * ## call order ## 
 *
 *
 */

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h> // strlen, strstr 
#include<memory.h> // memcpy 
#include<stdlib.h> // <-- calloc 
#include"dag.h"

#define BUF_LEN 1024 

// at first it only provides fixed schedule, consider to provider cron expression like function 
typedef enum {
	HOURLY, 
	DAILY, 
} dag_schedule; 


struct dag_order {
	struct dag_order * next; // next order info   
	const char* func_name;   // next execution function_name 
        dag_schedule schedule; 
}; 


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

	struct dag_order* ord = parse_dag_order_dsl(buf); 
	if (ord == NULL) {
		fprintf(stderr, "Failed to parse dag order\n"); 
		return NULL; 
	}

	return ord; 
}

// trim back and forward 
char* trim(const char* s) {
	// trim backward 
	
	char *ss = s; 

	size_t len = strlen(ss);
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


struct dag_order * parse_dag_order_dsl(const char* dsl) {
	fprintf(stdout, "parse dag order dsl start\n"); 
	struct dag_order* result; 
	char* dsll = dsl; 
	
	while(1) { 
		int pos = strstr(dsll, ">>"); 
		if (pos == NULL) { // might have only one Task 
			if (strlen(dsll) <= 0) {
				fprintf(stderr, "no task definition in dsl: %s\n", dsl); 
				return NULL; 
			}

			// lets trim 
			char* func_name = trim(dsll); 
			struct dag_order* ord = calloc(1, sizeof(struct dag_order)); 
			ord->func_name = func_name; 
			ord->next = NULL; 
			result = ord;
			return result; 			
		}

		size_t size = dsll - pos; 
		char func_name[64]; // 64byte length of func_name  
		memcpy(func_name, dsll, size);
	        func_name[size] = '\0'; 	
		dsll = func_name;

		struct dag_order* ord = calloc(1, sizeof(struct dag_order)); 	
		if (ord == NULL) {
			perror("calloc");
			return NULL; 
		}

		ord->func_name = func_name; 
		ord->next = NULL; 
		result->next = ord; 
	}

	fprintf(stdout, "Success to parse dag order dsl\n"); 
	return result; 
}
