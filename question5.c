/*
 * Tutorial 7 Signals and Data Structures for SOFE 3950: Operating Systems
 *
 * Copyright (C) 2024, <Mostafa Abedi, Nathaniel Manoj, Calvin Reveredo>
 * All rights reserved.
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


// Structure for each process
typedef struct proc {
    char name[256];
    int priority;
    int pid;
    int runtime;
} Proc;

// Structure for linked list node
typedef struct node {
    Proc process;
    struct node* next;
} Node;

// Structure for linked list
typedef struct queue {
    Node* front;
    Node* rear;
} Queue;

// Function to initialize a new queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Function to create a new node
Node* createNode(Proc process) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->process = process;
    newNode->next = NULL;
    return newNode;
}

// Function to add a process to the queue
void push(Queue* q, Proc process) {
    Node* newNode = createNode(process);
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

// Function to remove and return the front process from the queue
Proc pop(Queue* q) {
    if (q->front == NULL) {
        Proc empty;
        strcpy(empty.name, "");
        empty.priority = -1;
        empty.pid = -1;
        empty.runtime = -1;
        return empty;
    }
    Node* temp = q->front;
    Proc process = temp->process;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return process;
}

// Function to delete a process from the queue given its name
Proc delete_name(Queue* q, char* name) {
    Node* temp = q->front;
    Node* prev = NULL;
    Proc deleted;
    while (temp != NULL) {
        if (strcmp(temp->process.name, name) == 0) {
            if (prev == NULL) {
                q->front = temp->next;
            } else {
                prev->next = temp->next;
            }
            deleted = temp->process;
            free(temp);
            return deleted;
        }
        prev = temp;
        temp = temp->next;
    }
    strcpy(deleted.name, "");
    deleted.priority = -1;
    deleted.pid = -1;
    deleted.runtime = -1;
    return deleted;
}

// Function to print a process
void printProcess(Proc process) {
    printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n",
           process.name, process.priority, process.pid, process.runtime);
}


int main() {
    // Open the file for reading
    FILE* file = fopen("processes_q5.txt", "r");
    if (file == NULL) {
        //Error checking
        perror("Error opening file");
        return 1;
    }
    // Create a new queue
    Queue* q = createQueue();
    // Read the file line by line
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Create a new process from the line
        Proc process;
        sscanf(line, "%[^,], %d, %d,", process.name, &process.priority, &process.runtime);
        process.pid = 0;
        push(q, process);
    }
    // Close the file
    fclose(file);
    
// Iterate through the queue and execute processes
    Node* temp = q->front;
    while (temp != NULL) {
        if (temp->process.priority == 0) {
            // Execute process
            pid_t child_pid = fork();
            if (child_pid == 0) {
                // Child process
                execl(temp->process.name, temp->process.name, NULL);
                // If execl fails, terminate child process
                exit(1);
            } else if (child_pid > 0) {
                // Parent process
                temp->process.pid = child_pid;
                // Wait for process to terminate
                waitpid(child_pid, NULL, 0);
                // Send SIGINT to terminate process
                kill(child_pid, SIGINT);
                // Wait for process to fully terminate
                waitpid(child_pid, NULL, 0);
                // Print process information
                printProcess(temp->process);
            }
        }
        temp = temp->next;
    }

    // Iterate through the remaining processes in the queue
    while (q->front != NULL) {
        // Pop each item from the queue and execute process
        Proc process = delete_name(q, q->front->process.name);
        // Execute process
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // Child process
            execl(process.name, process.name, NULL);
            // If execl fails, terminate child process
            exit(1);
        } else if (child_pid > 0) {
            // Parent process
            process.pid = child_pid;
            // Wait for process to terminate
            waitpid(child_pid, NULL, 0);
            // Send SIGINT to terminate process
            kill(child_pid, SIGINT);
            // Wait for process to fully terminate
            waitpid(child_pid, NULL, 0);
            // Print process information
            printProcess(process);
        }
    }

    // Free memory allocated for the queue
    free(q);

    return 0;
}