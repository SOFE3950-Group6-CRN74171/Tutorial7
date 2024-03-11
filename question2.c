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
#include <string.h>

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

// Function to delete a process from the queue given its pid
Proc delete_pid(Queue* q, int pid) {
    Node* temp = q->front;
    Node* prev = NULL;
    Proc deleted;
    while (temp != NULL) {
        if (temp->process.pid == pid) {
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

// Function to print all processes in the queue
void printQueue(Queue* q) {
    Node* temp = q->front;
    while (temp != NULL) {
        printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n",
               temp->process.name, temp->process.priority, temp->process.pid, temp->process.runtime);
        temp = temp->next;
    }
}

int main() {
    // Open the file for reading
    FILE* file = fopen("processes.txt", "r");
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
        sscanf(line, "%[^,], %d, %d, %d", process.name, &process.priority, &process.pid, &process.runtime);
        push(q, process);
    }
    // Close the file
    fclose(file);
    
    printf("Processes in the queue before deletion:\n");
    // Print the queue before deletion
    printQueue(q);
    
    // Delete the process named 'emacs'
    delete_name(q, "emacs");
    
    // Delete the process with pid '12235'
    delete_pid(q, 12235);
    
    printf("\nProcesses being popped from the queue after deletion:\n");

    // Pop and print remaining processes in the queue
    while (q->front != NULL) {
        Proc process = pop(q);
        printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n",
               process.name, process.priority, process.pid, process.runtime);
    }
    printf("\nThe queue after pop (Should be empty):\n");
    printQueue(q);
    // Free memory allocated for the queue
    free(q);

    return 0;
}