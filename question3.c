/*
 * Tutorial 7 Signals and Data Structures for SOFE 3950: Operating Systems
 *
 * Copyright (C) 2024, <Mostafa Abedi, Nathaniel Manoj, Calvin Reveredo>
 * All rights reserved.
 *
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> 
#include <sys/wait.h>

int main() {
    pid_t pid;
    // Fork a child process
    pid = fork();
    // Fork failed
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    //Child process
    } else if (pid == 0) {
        // Execute the process using exec
        execl("./process", "process", NULL);
        // If execl returns, it means it failed
        perror("Exec failed");
        exit(EXIT_FAILURE);
    // Parent process
    } else {
        // Sleep for 5 seconds
        sleep(5);
        // Send SIGINT signal to the child process
        kill(pid, SIGINT);
        // Wait for the child process to terminate
        wait(NULL);
        printf("Parent process finished.\n");
    }

    return 0;
}
