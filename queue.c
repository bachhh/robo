#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int QueueIsEmpty(struct queue* queue){
    return (queue->count == 0) ? 1 : 0;
}

int QueueIsFull(struct queue* queue){
    return (queue->count == MAX_QUEUE_SIZE) ? 1 : 0;
}

void Enqueue(struct queue* queue, queueElementType element){
    if ( QueueIsFull(queue) == 0){
        queue->array[queue->tail] = element;
        queue->count++;
        queue->tail = (queue->tail >= MAX_QUEUE_SIZE - 1) ? 0 : queue->tail + 1;
    }
    else{
        printf("Error ! Queue is Full \n");
        exit(EXIT_FAILURE);
    }
}

queueElementType Dequeue(struct queue* queue){
    if (QueueIsEmpty(queue) == 0){
        queue->count--;
        queueElementType temp = queue->array[queue->head];
        queue->head = (queue->head >= MAX_QUEUE_SIZE - 1) ? 0 : queue->head + 1;
        return temp;
    }
    else {
        printf("Error ! Queue is Empty \n");
        exit(EXIT_FAILURE);
    }
}

struct queue* makeQueue(){
    struct queue* queue = malloc(sizeof(struct queue));
    queue->head = 0;
    queue->count = 0;
    queue->tail = 0;
    return queue;
}