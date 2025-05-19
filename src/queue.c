#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

queue *createQueue()
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue_if_id(queue *q, IF_ID *if_id)
{
    // Create a copy of the IF_ID structure to avoid using stack memory
    IF_ID *new_if_id = (IF_ID *)malloc(sizeof(IF_ID));
    if (new_if_id == NULL)
    {
        printf("Error: Failed to allocate memory for IF_ID structure\n");
        exit(EXIT_FAILURE);
    }

    // Copy the data
    new_if_id->instr = if_id->instr;
    new_if_id->pc = if_id->pc;
    new_if_id->next = NULL;

    // Add the new entry to the queue
    if (q->rear == NULL)
    {
        q->front = q->rear = new_if_id;
    }
    else
    {
        ((IF_ID *)q->rear)->next = new_if_id;
        q->rear = new_if_id;
    }
}

void enqueue_id_ex(queue *q, ID_EX *id_ex)
{
    // Create a copy of the ID_EX structure to avoid using stack memory
    ID_EX *new_id_ex = (ID_EX *)malloc(sizeof(ID_EX));
    if (new_id_ex == NULL)
    {
        printf("Error: Failed to allocate memory for ID_EX structure\n");
        exit(EXIT_FAILURE);
    }

    // Copy the data
    new_id_ex->instruction = id_ex->instruction;
    new_id_ex->pc = id_ex->pc;
    new_id_ex->opcode = id_ex->opcode;
    new_id_ex->r1 = id_ex->r1;
    new_id_ex->r2 = id_ex->r2;
    new_id_ex->r1_value = id_ex->r1_value;
    new_id_ex->r2_value = id_ex->r2_value;
    new_id_ex->immediate = id_ex->immediate;
    new_id_ex->next = NULL;

    // Add the new entry to the queue
    if (q->rear == NULL)
    {
        q->front = q->rear = new_id_ex;
    }
    else
    {
        ((ID_EX *)q->rear)->next = new_id_ex;
        q->rear = new_id_ex;
    }
}

IF_ID *dequeue_if_id(queue *q)
{
    if (q->front == NULL)
    {
        printf("Can't dequeue, queue is empty\n");
        return NULL;
    }

    IF_ID *temp = q->front;
    q->front = ((IF_ID *)q->front)->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    temp->next = NULL;
    return temp;
}

ID_EX *dequeue_id_ex(queue *q)
{
    if (q->front == NULL)
    {
        printf("Can't dequeue, queue is empty\n");
        return NULL;
    }

    ID_EX *temp = q->front;
    q->front = ((ID_EX *)q->front)->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    temp->next = NULL;
    return temp;
}

IF_ID *peek_if_id(queue *q)
{
    if (q->front == NULL)
    {
        printf("No peek, queue is empty\n");
        return NULL;
    }
    return q->front;
}

ID_EX *peek_id_ex(queue *q)
{
    if (q->front == NULL)
    {
        printf("No peek, queue is empty\n");
        return NULL;
    }
    return q->front;
}

int isEmpty(queue *q)
{
    return (q->front == NULL);
}

int getqueueSize_if_id(queue *q)
{
    int size = 0;
    IF_ID *current = q->front;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}

int getqueueSize_id_ex(queue *q)
{
    int size = 0;
    ID_EX *current = q->front;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}

void freequeue_if_id(queue *q)
{
    IF_ID *curr = q->front;
    IF_ID *next;
    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(q);
}

void freequeue_id_ex(queue *q)
{
    ID_EX *curr = q->front;
    ID_EX *next;
    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(q);
}

bool isqueueEmpty(queue *q)
{
    return q == NULL || q->front == NULL;
}

// Function to print the contents of a queue
void print_queue(queue *q)
{

    IF_ID *current = q->front;
    printf("Queue contents: ");
    while (current != NULL)
    {
        printf("0x%04X -> ", current->instr); 
        current = current->next;
    }
    printf("\n");
}
