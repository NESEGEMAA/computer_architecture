// QUEUE.h
#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include "types.h"

typedef struct queue
{
    void *front;
    void *rear;
} queue;

queue *createQueue();
void enqueue_if_id(queue *q, IF_ID *if_id);
void enqueue_id_ex(queue *q, ID_EX *id_ex);
IF_ID *dequeue_if_id(queue *q);
ID_EX *dequeue_id_ex(queue *q);
IF_ID *peek_if_id(queue *q);
ID_EX *peek_id_ex(queue *q);
int isEmpty(queue *q);
int getQueueSize_if_id(queue *q);
int getQueueSize_id_ex(queue *q);

#endif // QUEUE_H