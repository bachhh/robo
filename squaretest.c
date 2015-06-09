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
    go_straight(curr_coord, 60);
    spin(curr_coord, to_rad(90));
    go_straight(curr_coord, 60);
    spin(curr_coord, to_rad(90));
    go_straight(curr_coord, 60);
    spin(curr_coord, to_rad(90));
    go_straight(curr_coord, 60);
    spin(curr_coord, to_rad(90));

    return 0;
}
