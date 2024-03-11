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
        // Send SIGTSTP signal to the child process to suspend it
        kill(pid, SIGTSTP);
        // Sleep for 10 seconds
        sleep(10);
        // Send SIGCONT to the child process to resume it
        printf("Parent process suspended child process for 10 seconds.\n");
        kill(pid, SIGCONT);
        printf("Parent process resumed child process.\n");
        // Wait for the child process to terminate
        int status;
        waitpid(pid, &status, 0);

        printf("Parent process finished.\n");
    }

    return 0;
}
