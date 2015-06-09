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

int main(){
	connect_to_robot();
    initialize_robot();
	set_origin();
    double curr_coord[2] = {0, 0};
    int frontl, frontr, leftir, rightir, us;
    while (1){
    	get_front_ir_dists(&frontl, &frontr);
    	get_side_ir_dists(&leftir, &rightir);
    	us = get_us_dist();
    	printf("FRONTL %d, FRONTR %d, SIDEL%d, SIDER%d, US%d \n", frontl, frontr, leftir, rightir, us);
    }
    return 0;
}