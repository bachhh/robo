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
    int leftenc, rightenc;
    go_straight(curr_coord, 60);
    get_motor_encoders(&leftenc, &rightenc);
    printf("Encoder: %d, %d \n", leftenc, rightenc );
    printf("Distance = %f, %f \n", enc_to_dist(leftenc), enc_to_dist(rightenc));

    printf("Coord: X = %f, Y = %f \n", curr_coord[0], curr_coord[1]);
    return 0;
}
