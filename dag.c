/* ## DAG parse ## 
 * this component parse DAG dsl written in .dsl file. 
 * and generates order of DAG(Direct Asyclic Graph) and give it to scheduler
 */

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

#define BUF_LEN 1024 

struct dag_order {
	dag_order * next; 
	const char* func_name; 
}; 


struct dag_order* read_dag_dsl() {
	const char* dag_dsl_filename = "dag.dsl"; 
	int fd = open(dag_dsl_filename, O_RDONLY); 
	if (fd == -1){
		perror("open");
		return NULL; 
	}

	char buf[BUFLEN]; 
	int nbytes = read(fd, &buf, BUF_LEN);
	if (nbytes == -1) {
		perror("read"); 
		return NULL; 
	}

	struct dag_order* dag = parse_dag_order_dsl(buf); 
	if (dag == NULL) {
		fprintf(stderr, "Failed to parse dag order\n"); 
		return NULL; 
	}

	return dag_order; 
}


static dag_order* parse_dag_order_dsl(const char* dsl) {
	struct dag_order* result; 
	char* dsll = dsl; 
	
	while(1) { 
		int pos = strstr(dsll, ">>"); 
		if (pos == NULL) {
			perror("strstr");
			return NULL; 
		}

		size_t size = dsll - pos; 
		const char fuc_name[64]; // 64byte length of func_name  
		memcpy(func_name, dsll, size); 
		dsll = func_name;

		struct dag_order* ord = calloc(1, sizeof(dag_order)); 	
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
