#ifndef DAG_H
#define DAG_H 

// at first it only provides fixed schedule, consider to provider cron expression like function 
typedef enum {
	HOURLY, 
	DAILY, 
} dag_schedule; 

// TODO("name of this struct should be switch to task_order")
struct dag_order {
	struct dag_order* next; // next Task order info   
	const char* func_name;   // next execution function_name 
    dag_schedule schedule;   // schedule of DAG (will be implemented soon)
}; 

struct dag_order* read_dag_dsl(); 
struct dag_order* parse_dag_order_dsl(const char* dsl); 

#endif 
