
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include "picomms.h"

int parallel(){
	int i = 0;
    double leftir;
    double rightir;
    int parallel = 0;
    double error = 0.01 ;
	double time = 30.0;
	while (!parallel){
	    for ( i = 0; i < time; i++)
	    {
	        leftir += get_front_ir_dist(LEFT);
	    	rightir+= get_front_ir_dist(RIGHT);
	    }
		leftir = leftir/time;
		rightir = rightir/time;
	    if(leftir/time > rightir/time + error){
	    	set_motors(1, -1);
		for ( i = 0; i < time; i++){
			leftir += get_front_ir_dist(LEFT);
			rightir += get_front_ir_dist(RIGHT);
		}
		leftir = leftir/time;
		rightir = rightir/time;
}
	    else if(rightir/time > leftir/time + error){
	    	set_motors(-1, 1);
		for (i = 0; i < time; i++){
			leftir += get_front_ir_dist(LEFT);
			rightir += get_front_ir_dist(RIGHT);
		}
		leftir = leftir/time;
		rightir = rightir/time;
	}
	    else{
	    	parallel = 1;
	    	set_motors(0, 0);
	   }
	printf("LEFTIR %f, RIGHTIR %f \n", leftir/time, rightir/time);
	usleep(300000);
	set_motors(0, 0);
	}
	return parallel;
}

int main(){
	connect_to_robot();
    initialize_robot();
    set_ir_angle(LEFT, 45);
    set_ir_angle(RIGHT, -45);
	int i =	parallel();
	printf("Parallel: %d \n", i);
	return 0;
}
