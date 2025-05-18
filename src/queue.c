#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "queue.h"

queue *createQueue()
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue_if_id(queue *q, IF_ID *if_id)
{
    // Add the process directly to the queue without cloning
    if (q->rear == NULL)
    {
        q->front = q->rear = if_id;
    }
    else
    {
        ((IF_ID *) q->rear)->next = if_id;
        q->rear = if_id;
    }
}

void enqueue_id_ex(queue *q, ID_EX *id_ex)
{
    // Add the process directly to the queue without cloning
    if (q->rear == NULL)
    {
        q->front = q->rear = id_ex;
    }
    else
    {
        ((ID_EX *) q->rear)->next = id_ex;
        q->rear = id_ex;
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