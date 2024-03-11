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
    struct Node* next;
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
    q->rear->next = (struct Node*)newNode;
    q->rear = newNode;
}

// Function to print all processes in the queue
void printQueue(Queue* q) {
    Node* temp = q->front;
    while (temp != NULL) {
        printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n",
               temp->process.name, temp->process.priority, temp->process.pid, temp->process.runtime);
        temp = (Node*)temp->next;
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
    printf("Processes in the queue:\n");
    // Print the queue
    printQueue(q);

    return 0;
}

