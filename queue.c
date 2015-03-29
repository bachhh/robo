#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "picomms.h"
#include "maplib.h"



struct queue{
	struct node* array[30];
	int head;
	int tail;
};

void QueueEnter( struct queue* queue, struct node* node){
	queue->array[tail] = node;
	queue->tail += 1;
}

struct node* QueueRemove( struct queue* queue){
	queue->head += 1;
	return queue->array[head - 1];
}
	
int QueueIsEmty(struct queue* queue){
	return (queue->head == 0 && queue->tail == 0 || queue->head == queue->tail) ? 1 : 0; 
}

int QueueIsFull(struct queue* queue){
	return (queue->tail == 30) ? 1 : 0;
}


void ShortestPath(struct node* startnode){
	struct queue* nqueue = malloc(sizeof(struct queue));
	nqueue->head = 0;
	nqueue->tail = 0;
	QueueEnter(nqueue, startnode);
	startnode->discovered = 1;
	while ( QueueIsEmpty(nqueue) == 0){
		tempnode = QueueRemove( nqueue);
		int i;
		for (i = 0; i < 4; i++){
			if ( tempnode->adjacent[i] != 0 ){
				if (tempnode->adjacent[i]->discovered != 1){
					QueueEnter(nqueue, tempnode->adjacent[i]);
					tempnode->adjacent[i]->parent = tempnode;
				}
			}
		}
	}
}