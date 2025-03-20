/*
* Thread-Safe Stack Using Pthreads (With Debugging Output)
* This program demonstrates a stack implemented using a linked list,
* with multiple threads performing concurrent push and pop operations.
* A mutex lock ensures thread safety and prevents race conditions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10  // Number of concurrent threads (reduce for debugging)
#define ITERATIONS 500  // Number of push/pop cycles each thread executes

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex to ensure thread safety

// Linked list node structure
typedef int value_t; // Stack stores integer values

typedef struct Node {
    value_t data;  // Value stored in the node
    struct Node *next; // Pointer to the next node in the stack
} StackNode;

// Global stack pointer (shared among all threads)
StackNode *top = NULL;

// Function declarations
void push(value_t v);  // Push an element onto the stack
value_t pop();         // Pop an element from the stack
int is_empty();        // Check if the stack is empty
void *testStack(void *args); // Function for threads to execute

int main(void) {
    pthread_t threads[NUM_THREADS]; // Array to store thread IDs

    // Create multiple threads
    for(int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, testStack, NULL) != 0) {
            perror("Error creating thread"); // Print error if thread creation fails
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for(int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error joining thread"); // Print error if thread joining fails
            exit(EXIT_FAILURE);
        }
        printf("Completed Thread: %d\n", i + 1); // Print confirmation of thread completion
    }

    return 0; // Successful program execution
}

// Function executed by each thread
void *testStack(void *args) {
    pthread_t thread_id = pthread_self(); // Get the unique thread ID

    for(int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex); // Lock the mutex before modifying the stack

        // Push three values onto the stack
        printf("[Thread %lu] Pushing: %d\n", thread_id, i);
        push(i);

        printf("[Thread %lu] Pushing: %d\n", thread_id, i + 1);
        push(i + 1);

        printf("[Thread %lu] Pushing: %d\n", thread_id, i + 2);
        push(i + 2);

        // Pop three values from the stack
        int v1 = pop();
        printf("[Thread %lu] Popped: %d\n", thread_id, v1);

        int v2 = pop();
        printf("[Thread %lu] Popped: %d\n", thread_id, v2);

        int v3 = pop();
        printf("[Thread %lu] Popped: %d\n", thread_id, v3);

        pthread_mutex_unlock(&mutex); // Unlock the mutex after modifying the stack
    }
    return NULL; // Return NULL as required by pthreads
}

// Function to push a value onto the stack
void push(value_t v) {
    // Allocate memory for a new node
    StackNode *new_node = malloc(sizeof(StackNode));
    if (!new_node) {
        perror("Memory allocation failed"); // Print error if memory allocation fails
        exit(EXIT_FAILURE);
    }
    new_node->data = v; // Assign value to the new node
    new_node->next = top; // Link the new node to the current top of the stack
    top = new_node; // Update the top pointer
}

// Function to pop a value from the stack
value_t pop() {
    if (is_empty()) {
        return 0; // If stack is empty, return 0 (avoid dereferencing NULL)
    }
    StackNode *temp = top; // Store the current top node
    value_t data = temp->data; // Retrieve data from the top node
    top = top->next; // Move the top pointer to the next node
    free(temp); // Free memory of the popped node
    return data; // Return the popped value
}

// Function to check if the stack is empty
int is_empty() {
    return top == NULL; // Return 1 if stack is empty, 0 otherwise
}
