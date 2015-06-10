#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include "picomms.h"       
#include "movelib.h"
#include "queue.h"

/*
###             Mapping Standard            ###
###                                         ###
____________
|13 14 15 16|
|9  10 11 12|
|5  6  7  8 | 
|1  2  3  4 | 
|0|__________

###                                          ###
###                                          ###



    Integrate the mapping from the midterm task into the mapping of the maze.
    Particulary, mapping will  be integrated into:
            move_to_node()
            move_to()  

    Because each time you carry out some movement you have to log your mapping
            
    With more insight:  move_to_node() will take an other pointer to the robot currently X Y coordinates and 
                        log them into it's node's x and y
            Which in turn, the map() function will also take a pointer to X Y coordinates and pass down to move_to_node()
                        because move_to_node() is called by map()

    Maybe the variable "angle" can be regarded as a global variable for convenience. 
    I found myself add int* angle as a parameter to too many functions.

    Otherwise, try to use pointer as much as possible instead of globals
                                                                                                        


*/


struct node* nodes[17];

void initialize_maze(){
    int i;
    for (i= 0; i < 17; ++i)
    {
        nodes[i] = malloc(sizeof(struct node));
        nodes[i]->name = i;
        nodes[i]->visited = 0;
        nodes[i]->discovered = 0;
        memset(nodes[i]->adjacent, 0, sizeof(nodes[i]->adjacent));
        if (i != 0){
           nodes[i]->x = ((i - 1) % 4) * 60;
           nodes[i]->y = ( (int) ( (i - 1) / 4) + 1) * 60;
        }
    }
}



/*
    If you add an adjacent node to the adjacent[] of your current node
    The first element adjacent[0] might already have the address of other node
    You want to find some available slot for you to add the address of adjacent node to your current node
    Here is the solution
*/
int available_adjacent(struct node* node){
    int i;
    for (i = 0; i < 4; i++){
        if (node->adjacent[i] == 0)  
            return i;
    }
    return i;
} 

/*
    Your adjacent nodes change depends on the angle you are facing

    E.g: If you are facing SOUTH and you are at node 7
            Your LEFT adjacent node is 8
            Your RIGHT adjacent node is 6
            Your FRONT adjacent node is 3
    The following codes provides a set of functions for solving this problem.
*/
int node_in_front(double angle, struct node* currentnode){
    // Caculate the index of node in the "nodes[]" array, based on it's face_angle
    angle = to_degree(angle);
    if (currentnode->name == 0)
        return 1;
    if (angle < 30 && angle > -30)
        return currentnode->name + 4;
    else if (angle < 120 && angle > 60)
        return currentnode->name + 1;
    else if (angle > -120 && angle < -60)
        return currentnode->name - 1;
    else if (angle < -150 || angle > 150) 
        return currentnode->name - 4;
    else return 17; // Basically a "ghost" node as default value, preventing segmentation fault.
}

int node_on_left(double angle, struct node* currentnode){
    angle = to_degree(angle);
    if (angle < 30 && angle > -30)
        return currentnode->name - 1;
    else if (angle < 120 && angle > 60)
        return currentnode->name + 4;
    else if (angle > -120 && angle < -60)
        return currentnode->name - 4;
    else if (angle < -150 || angle > 150) 
        return currentnode->name + 1;
    else return 17;
}

int node_on_right(double angle, struct node* currentnode){
    angle = to_degree(angle);
    if (currentnode->name == 0)
        return -1;
    if (angle < 30 && angle > -30)
        return currentnode->name + 1;
    else if (angle < 120 && angle > 60)
        return currentnode->name - 4;
    else if (angle > -120 && angle < -60)
        return currentnode->name + 4;
    else if (angle < -150 || angle > 150) 
        return currentnode->name - 1;
    else return 17;
}

void move_to_node(double curr_coord[2], struct node* node){
    printf("\t \t ### Moving to node: %d  ###\n",node->name );
    printf("Moving to coord: x %f y %f \n",node->x, node->y );
    move_to(curr_coord, node->x, node->y);
    printf(" Arrived at node[%d] ! \n", node->name);

    // Start mapping walls
    struct node* currentnode = node;
    currentnode->visited = 1;
    int currentfront = node_in_front(face_angle, currentnode);
    int currentleft = node_on_left(face_angle, currentnode);
    int currentright = node_on_right(face_angle, currentnode);
    int i = available_adjacent(currentnode);
    int j;

    if (no_wall_front() == 1){
        if (nodes[currentfront]->visited == 0){
            currentnode->adjacent[i] = nodes[currentfront]; 
            i = available_adjacent(currentnode);           
            j = available_adjacent(nodes[currentfront]);
            nodes[currentfront]->adjacent[j] = currentnode;
        }
        printf("There is no wall front , US dist:  %d \n", get_us_dist());
    }
    else if(no_wall_front() == 0){
        parallel(curr_coord);
    }
    if (no_wall_left() == 1){
        if (nodes[currentleft]->visited == 0){
            currentnode->adjacent[i] = nodes[currentleft];
            i = available_adjacent(currentnode);
            j = available_adjacent(nodes[currentleft]);
            nodes[currentleft]->adjacent[j] = currentnode;  
        }
        printf("There is NO wall left ! LEFT IR : %d\n", get_side_ir_dist(LEFT));
    }
    if (no_wall_right() == 1){
        if (nodes[currentright]->visited == 0){
            currentnode->adjacent[i] = nodes[currentright];
            i = available_adjacent(currentnode);
            j = available_adjacent(nodes[currentright]);
            nodes[currentright]->adjacent[j] = currentnode;        
        }
        printf("There is NO wall right ! RIGHT IR : %d \n", get_side_ir_dist(RIGHT));
    }
    printf("Checking ALL for node[%d] wall done!\n", node->name);

}
void return_to_node(double curr_coord[2], struct node* returnnode){
    printf("\t \t ### Returning to node[%d] ###\n", returnnode->name);
    move_to(curr_coord, returnnode->x, returnnode->y);
    if(no_wall_front() == 0){
        parallel(curr_coord);
    }
    usleep(1000);
}

void map(double curr_coord[2], struct node* currentnode){
    int i;
    move_to_node(curr_coord, currentnode);
    printf("Move to node[%d] DONE! \n", currentnode->name);
    for (i = 0; i < 4; i++){
        if (currentnode->adjacent[i] != 0 && currentnode->adjacent[i]->visited != 1){
            map(curr_coord, currentnode->adjacent[i]);
            return_to_node(curr_coord, currentnode);
        }
    }

}

void breadthFirstSearch(struct node* startnode){
    struct queue* queue = makeQueue();
    Enqueue(queue, startnode);
    startnode->discovered = 1;
    while (QueueIsEmpty(queue) != 1){
        struct node* tempnode = Dequeue(queue);
        int i;
        if (tempnode->name == 16)
            break;
        for (i = 0; i < 4; i++){
            if (tempnode->adjacent[i] != 0 && tempnode->adjacent[i]->discovered != 1){
                Enqueue(queue, tempnode->adjacent[i]);
                tempnode->adjacent[i]->parent = tempnode;
                tempnode->adjacent[i]->discovered = 1;
            }
        }
    }
}

void reversePath(struct node* node){
    if (node->name == 16 )  
        node->child = NULL;
    if (node->name == 0 )   
        node->parent = NULL;
    while(node->parent){
        printf("node: %d \n",node->name);
        node->parent->child = node;
        node = node->parent;
    }
    printf("Done reversePath");
}

void printPath(struct node* node){
    if (node->name == 16){
        printf(" %d END ! \n", node->name);
    }
    else{
        printf(" %d ->", node->name);
        printPath(node->child);
    }
}
// Safe verision of traversing
void mazeRace(double curr_coord[2], struct node* node){
    while(node->child){
        move_to(curr_coord, node->child->x, node->child->y);
        node = node->child;
    }
}

struct point* connect_node(struct point* tail,struct node* from, struct node* to){
    int degree_of_spacing = 5;       // Divides 2 nodes into 'degree'- number  of small sections
    int sumx = to->x - from->x;
    int sumy = to->y - from->y;
    int marginx = sumx/degree_of_spacing;
    int marginy = sumy/degree_of_spacing;
    int i;
    for (i = 1; i <= degree_of_spacing; i++){        
        struct point* newpoint = malloc(sizeof(struct point));
        newpoint->x = from->x + marginx * i;
        newpoint->y = from->y + marginy * i;
        tail->next = newpoint;
        tail = newpoint;
    }
    return tail;
}   

void build_path(struct point* tail, struct node* startnode){
    while(startnode->child){
        struct point* temp = connect_node(tail, startnode, startnode->child);
        startnode = startnode->child;
        tail = temp;
    }
    tail->next = NULL; 
}

// Fast race versionS
void maze_race(double curr_coord[2], struct point* startpoint){
    double dx = startpoint->next->x - startpoint->x;
    double dy = startpoint->next->y - startpoint->y;
    double distance = fabs(sqrt(dx*dx + dy*dy));

    while (startpoint->next->next != NULL){
        if (distance < 27){
            startpoint = startpoint->next; // If we are too close aim for the next node
            printf("Switched point!!! \n ");
        } 
        distance = race_to(curr_coord, startpoint->next->x, startpoint->next->y);
    }
    while (distance >= 2){
        printf("Approaching Final! \n");
        distance = race_to(curr_coord, startpoint->x, startpoint->y);
    }
    set_motors(0,0);
    printf(" ### FINISHED ### \n");
}

int main(){

    connect_to_robot();
    initialize_robot();
    set_origin();
    set_ir_angle(LEFT, -45);
    set_ir_angle(RIGHT, 45);
    initialize_maze();
    reset_motor_encoders();
    int i;
    for (i = 0; i < 17; i++){
        set_point(nodes[i]->x, nodes[i]->y);
    }
    double curr_coord[2] = {0, 0};
    map(curr_coord, nodes[0]);
    breadthFirstSearch(nodes[0]);
    reversePath(nodes[16]);
    printPath(nodes[0]);

    struct point* tail = malloc(sizeof(struct point));
    tail->x = nodes[0]->x;
    tail->y = nodes[0]->y;
    struct point* startpoint = tail;

    build_path(tail, nodes[0]);
    
    // Traverse to end node.
    while(tail->next){
        set_point(tail->x, tail->y); // Visual display for Simulator only.
        tail = tail->next;
    }
    tail->next = NULL; // Final node point to null.
    printf("tail: X = %f Y = %f \n", tail->x, tail->y);
    parallel(curr_coord);
    spin(curr_coord, to_rad(180));
    
    sleep(2);
    set_ir_angle(LEFT, 45);
    set_ir_angle(RIGHT, -45);

    mazeRace(curr_coord, nodes[0]);
    return 0;
}