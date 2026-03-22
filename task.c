/* ## Task ## 
 * handle Actual functions as a unit of Task 
 * if you write 1 Task, it will configure the part of "DAG" 
 * 
 * 
 * Lets implement actual executor of Task. 
 * 1) at first we will just use system python execution, task will be a single .py file. 
 * 2) executor just execute python file 
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

char* make_py_file_name(char* name) { 
    char py_extension[] = ".py"; 
    char arr[64]; 
    strcpy(arr, name);
    
    strcat(arr, py_extension);
    char* full_file_name = strdup(arr); 
    return full_file_name; 
}

// fork() -> execlp() -> waitpid() 
int execute_task(const char* name) { 
    fprintf(stdout, "start execute task - name: %s\n", name);

    pid_t pid = fork(); 
    if (pid < 0) { 
        perror("fork");
        return -1; 
    }

    // child process 
    if (pid == 0) { 
        const char* file_name = make_py_file_name(name); 
        fprintf(stdout, "file_name: %s\n", file_name); 
        execlp("python3", "python3", file_name, (char*) NULL); 
        perror("execlp");
        _exit(127);
    }

    int status = 0; 
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1; 
    }

    if (WIFEXITED(status)) { 
        int exit_code = WEXITSTATUS(status);
        fprintf(stdout, "task exited with code: %d\n", exit_code); 
        return exit_code; 
    }

    if (WIFSIGNALED(status)) { 
        int signal_no = WTERMSIG(status);
        fprintf(stdout, "task terminated with signal: %d\n", signal_no);
        return -1; 
    }

    return -1; 
}