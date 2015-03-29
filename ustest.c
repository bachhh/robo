#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h> 
#include "picomms.h"       // Compile with -lm flag.
#include "movelib.h"
int no_wall_front(){
    int i = 0;
    double wall = 0;
    // Taking average of multiple readings is more reliable
    for (i = 0; i < 10; i++)
         wall += get_us_dist();
    return (wall/10 < 40 ) ? 0 : 1;
}

int main(){

    connect_to_robot();
    initialize_robot();
	printf("US test: Is there wall front %c? ", ( no_wall_front() == 1) ? 'N' : 'Y');
	printf("(US dist :  %d)\n", get_us_dist() );
}