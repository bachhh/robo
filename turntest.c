#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include "picomms.h"       
#include "movelib.h"
#include "maplib.h"

struct node* nodes[17];

int main(){
	connect_to_robot();
    initialize_robot();
	set_origin();
    double curr_coord[2] = {0, 0};
   	initialize_maze();
   	nodes[0]->adjacent[0] = nodes[1];
   	nodes[1]->adjacent[0] = nodes[2];
   	nodes[2]->adjacent[0] = nodes[6];
   	nodes[0]->child = nodes[1];
   	nodes[1]->child = nodes[5];
   	nodes[5]->child = nodes[9];
   	nodes[9]->child = nodes[13];
   	nodes[13]->child = nodes[14];
   	nodes[14]->child = nodes[10];
   	nodes[10]->child = nodes[6];
   	nodes[6]->child = nodes[2];
   	nodes[2]->child = nodes[3];


   	struct point* tail = malloc(sizeof(struct point));
    tail->x = nodes[0]->x;
    tail->y = nodes[0]->y;
    struct point* startpoint = tail;

    build_path(tail, nodes[0]);

   	while(tail->next){
        set_point(tail->x, tail->y); // Visual display for Simulator only.
        tail = tail->next;
    }
    tail->next = NULL; // Final node point to null.
    maze_race(curr_coord, startpoint);

    return 0;
}