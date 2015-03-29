#include <stdio.h>
#include "queue.h"

void QueueIsEmpty(struct queue* queue){
    return (queue->count == 0) ? 1 : 0;
}

void QueueIsFull(struct queue* queue){
    return (queue->count == MAX_QUEUE_SIZE) ? 1 : 0;
}

void Enqueue(struct queue* queue, queueElementType element){
    if ( QueueIsFull(queue) != 0){
        queue->array[queue->tail] = element;
        queue->count++;
        queue->tail = (queue->tail >= MAX_QUEUE_SIZE - 1) ? 0 : queue->tail + 1;
    }
    else
        printf("Error ! Queue is Full \n");
}

queueElementType Dequeue(struct queue* queue, queueElementype element){
    if (QueueIsEmpty(queue) != 0){
        queue->count--;
        queueElementType temp = queue->array[head];
        queue->head = (queue->head >= MAX_QUEUE_SIZE - 1) ? 0 : -> queue->head + 1;
        return temp;

    }
    else 
        printf("Error ! Queue is Empty \n");
}