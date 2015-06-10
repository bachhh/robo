#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include "picomms.h"       

int no_wall_left(double *reg){
    int i = 0;
    double wall = 0;
    for (i = 0; i < 100; i++)
    {
        wall += get_front_ir_dist(LEFT);
    }
    *reg = wall/100;
    return (wall/100 < 35) ? 0 : 1;
}

int no_wall_right(double *reg){
    int i = 0;
    double wall = 0;
    for ( i = 0; i < 100; i++)
    {
        wall += get_front_ir_dist(RIGHT);
    }
    *reg = wall/100;
    return (wall/100 < 35) ? 0 : 1;
}

int no_wall_front(double *reg){
    int i = 0;
    double wall = 0;
    for (i = 0; i < 100; i++){
        wall += get_us_dist();
    }
    *reg = wall/100;
    return (wall/100 < 40 ) ? 0 : 1;
}

int main(){
    connect_to_robot();
    initialize_robot();
    set_ir_angle(LEFT, -45);
    set_ir_angle(RIGHT, 45);
    double reg1 = 0, reg2 = 0, reg3 = 0;
    while (1){
        printf("FRONT: %d|%f , RIGHT: %d|%f, LEFT %d|%f \n", no_wall_front(&reg1), reg1, no_wall_right(&reg2), reg2, no_wall_left(&reg3), reg3);
    }
    return 0;
}