#include <stdio.h>
#include "queue.h"

int main(){
	struct queue* queue = makeQueue();
	int i;
	for (i = 0; i < 20; i++)
		Enqueue(queue, i);
	//for (i = 0; i < 20; ++i)
	//	Enqueue(queue, i);
	for (i = 0; i < 30; i++)
		printf("Queue element: %d \n", Dequeue(queue));
	return 0;
}