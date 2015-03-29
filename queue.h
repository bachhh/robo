#include "maplib.h"

#define MAX_QUEUE_SIZE 100

typedef struct node* queueElementType;

struct queue {
    queueElementType array[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int count;
};