# cflow
A C-lang written Airflow like worfkow management opensource project.       
just for fun.


## how to compile? 


## commit convention 
- just simply use feat,fix,refactor,doc 



## Role 
- Schedulder
- - handle all the execution of workflow, and also resource management. 
- Dag
- - handle the execution order of tasks, which forms DAG(Direct Acyclic Graph)
- Context
- - maintain small chunk of data between tasks. 
- Task 
- - A single unit of execution -> one function 
- Log 
- - Manage Execution log of Task 


## build 
`gcc   -Wall -Wextra -pthread -O2 -g *.c -o cflow`

## execute 
1) `./cflow`
2) type `RUN`