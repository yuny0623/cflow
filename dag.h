#ifndef DAG_H
#define DAG_H 

struct dag_order* read_dag_dsl(); 
struct dag_order* parse_dag_order_dsl(const char* dsl); 

#endif 
