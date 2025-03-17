/*
* Stack containing race conditions
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Linked list node
typedef int value_t;

typedef struct Node
{
    value_t data;
    struct Node *next;
} StackNode;

// Stack function declarations
void push (value_t v, StackNode **top);
value_t pop ( StackNode **top);
int is_empty( StackNode *top);

int main(void) {

	pthread_t threads[200];

	for(int i = 0; i < 200; i++)
	{
		if(pthread_create(&threads[i], NULL, testStack, NULL) != 0)
			break;
	}

	for(int i = 0; i < num; i++)
	{
		if(pthread_join(threads[i],NULL) != 0)
			break;
		printf("Running Thread: %d\n", i + 1);
	}

    return 0;
}

void * testStack(void * args) {
    pthread_mutex_lock(&mutex);
    StackNode *top = NULL;
    int data = 1;

    for(int i = 0; i < 500; i++) {
        for(int j = 0; j < 3; j++) {
            push(data + j, &top);
            data++;
        }
        int value = pop(&top);
        value = pop(&top);
        value = pop(&top);
    }
    pthread_mutex_unlock(&mutex);
}

// Stack function definitions
void push(value_t v, StackNode **top) {
    StackNode * new_node = malloc(sizeof(StackNode));
    new_node->data = v;
    new_node->next = *top;
    *top = new_node;
}

value_t pop(StackNode **top) {
    if (is_empty(*top)) return (value_t)0;
    value_t data = (*top)->data;
    StackNode * temp = *top;
    *top = (*top)->next;
    free(temp);
    return data;
}

int is_empty(StackNode *top) {
    if (top == NULL) return 1;
    else return 0;
}
