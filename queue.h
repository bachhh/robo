#include "maplib.h"

#define MAX_QUEUE_SIZE 100

typedef struct node* queueElementType;

struct queue {
    queueElementType array[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int count;
};

void QueueIsEmpty(struct queue* queue);
void QueueIsFull(struct queue* queue);
void Enqueue(struct queue* queue, queueElementType element);
queueElementType Dequeue(struct queue* queue, queueElementype element);