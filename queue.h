#include "maplib.h"

#define MAX_QUEUE_SIZE 20

typedef struct node* queueElementType;

struct queue {
    queueElementType array[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int count;
};

int QueueIsEmpty(struct queue* queue);
int QueueIsFull(struct queue* queue);
void Enqueue(struct queue* queue, queueElementType element);
queueElementType Dequeue(struct queue* queue);
struct queue* makeQueue();