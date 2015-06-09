#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h> 
#include "picomms.h"
#include "movelib.h"

// cm, vary on individual robot
#define R_WHEEL 4.7548
// cm, vary on individual robot     
double width = 22.5;
double face_angle;

int prevenc[2] = {0, 0};

// Trivial maths functions.

double to_rad(double degree) {
    return degree * (M_PI/180);
}

double to_degree(double rad){
    return rad* (180/M_PI);
}

double angle_change(int prevenc[2]);

double enc_to_dist(int enc) {
    // Just a rough estimation based on observing the simulator
    // Wheel diameter = 76mm
    // 1 enc = 1 degree turn
    // > 2*M_PI*76 per 360 enc
    // Unit is cm
    enc = (double) enc;
    return (enc * ((2*M_PI*R_WHEEL)/360));
}


//Position tracking on servo encoder reading.

double position_tracker(double curr_coord[2]) {
    //  Function update global var face_angle.
    //  Function update current coordinates.
    //  Return the distance traveled.

    int currenc[2] = {0, 0};
    get_motor_encoders(&currenc[LEFT], &currenc[RIGHT]);
    double dl = enc_to_dist(currenc[LEFT] - prevenc[LEFT]);
    double dr = enc_to_dist(currenc[RIGHT] - prevenc[RIGHT]);
    double dx = 0, dy = 0;
    double dangle = ( dl - dr )/ width;

    if (dangle != 0) {
        double rl = dl / dangle;
        double rr = dr / dangle;
        double rm = ( rl + rr ) / 2;
        dx = rm * ( cos(face_angle) - cos(face_angle + dangle) ); 
        dy = rm * ( sin(face_angle + dangle) - sin(face_angle) );
        face_angle += dangle;
        face_angle += (face_angle > to_rad(180)) ? -to_rad(360) : (face_angle < -to_rad(180)) ? to_rad(360) : 0;
    }
    else {
        dy = dl * cos(face_angle);
        dx = dr * sin(face_angle);
    }
    // Encoders update
    prevenc[LEFT] = currenc[LEFT]; 
    prevenc[RIGHT] = currenc[RIGHT];
    // Coordinates update
    
    curr_coord[0] += dx;       
    curr_coord[1] += dy;

    return sqrt(dx*dx + dy*dy);
}
/*
    Basic moves functions, basically turning fix angle and going fix distance straight.

    Every movement functions has array double curr_coord[2] as parameter.

    curr_cord[2] = {x_coordinates, y_coordinates};
*/

void spin(double curr_coord[2], double angle){
    if (angle == 0) 
        return;
    printf("Start Spinning: from %f, with :%f\n",to_degree(face_angle), to_degree(angle));
    double initial_angle = face_angle;
    int speed = 15

    ;
    int tempspeed = 0;
    double angle_turned = 0; 
    double abs_angle = fabs(angle);
    double side = angle/fabs(angle);
    int tempenc[2] = {0, 0};

    while (angle_turned < abs_angle){
        get_motor_encoders(&tempenc[0], &tempenc[1]);
        double dl = enc_to_dist(tempenc[0] - prevenc[0]);
        double dr = enc_to_dist(tempenc[1] - prevenc[1]);
        face_angle += ( dl - dr )/ width;

        if (abs_angle < to_rad(30)){
            tempspeed = side * 1;
            set_motors(tempspeed, -tempspeed);
        }
        else if(angle_turned >= fabs(0.92*angle)){
            tempspeed = (tempspeed < 2) ? side : side * speed * (1 - fabs(angle_turned/angle));
            set_motors(tempspeed, -tempspeed);
        }
        else {
            set_motors(side*speed, side*-speed);
        }
        angle_turned = fabs(face_angle - initial_angle);
        prevenc[0] = tempenc[0];
        prevenc[1] = tempenc[1];
       //printf("Monitoring: angle: %f  X: %f , Y: %f \n", to_degree(face_angle), curr_coord[0], curr_coord[1] );

    }
    position_tracker(curr_coord);
    set_motors(0, 0);
    printf("(Spinning done ! %f \n", to_degree(face_angle) );
}


void go_straight(double curr_coord[2], double distance){
    int speed = 40;
    int tempspeed = 0;
    double distance_traveled = 0;

    while (distance_traveled < distance){
        if (distance_traveled > 0.96*distance){
            tempspeed = (tempspeed < 2) ? 1 : speed * (1 - distance_traveled/distance);
        }
        else {
            tempspeed = speed;
        }
        set_motors(tempspeed, tempspeed);

        distance_traveled +=position_tracker(curr_coord);
    }
    position_tracker(curr_coord);
    set_motors(0, 0);
    usleep(200);
}

void move_to(double curr_coord[2], double x, double y){
    double dx = x - curr_coord[0];
    double dy = y - curr_coord[1];

    double targetA = atan2(dx, dy);
    
    double dangle = targetA - face_angle;
    dangle += (dangle > to_rad(180)) ? -to_rad(360) : (dangle < -to_rad(180)) ? to_rad(360) : 0;

    spin(curr_coord, dangle);
    go_straight(curr_coord, fabs(sqrt(dx*dx + dy*dy)));
    printf("Moving Done : X = %f, Y = %f, face_angle = %f \n", curr_coord[0], curr_coord[1], to_degree(face_angle));
    sleep(0.3);
}



int no_wall_left(){
    int i = 0;
    double wall = 0;
    for (i = 0; i < 10; i++)
    {
        wall += get_front_ir_dist(LEFT);
    }
    return (wall/10 < 35) ? 0 : 1;
}

int no_wall_right(){
    int i = 0;
    double wall = 0;
    for ( i = 0; i < 10; i++)
    {
        wall += get_front_ir_dist(RIGHT);
    }
    return (wall/10 < 35) ? 0 : 1;
}

int no_wall_front(){
    int i = 0;
    double wall = 0;
    // Taking average of multiple readings is more reliable
    for (i = 0; i < 10; i++)
         wall += get_us_dist();
    return (wall/10 < 40 ) ? 0 : 1;
}

void centering(){
    set_motors(-14, -14);
    sleep(3);
    reset_motor_encoders();
}