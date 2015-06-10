#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h> 
#include "picomms.h"
#include "movelib.h"

// ### Physical parameters.
#define R_WHEEL 4.9548 // cm, vary on individual robot, 4.7548 is ideal for the simulation.     
double width = 23.7; // cm, vary on individual robot, 22.5 is ideal for the simulation.
double face_angle; // Some function already has 3-4 parameters, I feel leaving face_angle is better.

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
    // Unit is cm
    return (( (double) enc ) * ((2*M_PI*R_WHEEL)/360));
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
    int speed = 15;
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
    usleep(10000);
}


void go_straight(double curr_coord[2], double distance){
    int speed = 30;
    int tempspeed = 0;
    double distance_traveled = 0;

    while (distance_traveled < distance){
        if (distance_traveled > 0.90*distance){
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
    usleep(50000);
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
}

// ### Experimental racing function
double race_to(double curr_coord[2], double x, double y){
    double steering = 1.8;                 // Ratio between the speed of each wheels.
    double error_margin_angle = 3;      // Degrees
    double speed = 70;
    double dx = x - curr_coord[0];
    double dy = y - curr_coord[1];
    
    double targetA = atan2(dx, dy);
    double dangle = targetA - face_angle;
    dangle += (dangle > to_rad(180)) ? -to_rad(360) : (dangle < -to_rad(180)) ? to_rad(360) : 0;

    double distance = fabs(sqrt(dx*dx + dy*dy));
    // If we need to turn, perform steering maneuver
    if(fabs(dangle) >= to_rad(error_margin_angle)){
        if (to_degree(dangle) > 0){
            set_motors( speed*steering, speed/steering);
        }
        else {
            set_motors(speed/steering, speed*steering);
        }
        printf("Steering: %f \n", to_degree(dangle));
    }
    else { // Otherwise just go straight;
        set_motors(speed, speed);
    }
    position_tracker(curr_coord);
    return distance;
}


// Taking average of multiple readings is more reliable

int no_wall_left(){
    int i = 0;
    double wall = 0;
    for (i = 0; i < 25; i++)
    {
        wall += get_front_ir_dist(LEFT);
    }
    return (wall/25 < 35) ? 0 : 1;
}

int no_wall_right(){
    int i = 0;
    double wall = 0;
    for ( i = 0; i < 25; i++)
    {
        wall += get_front_ir_dist(RIGHT);
    }
    return (wall/25 < 35) ? 0 : 1;
}

int no_wall_front(){
    int i = 0;
    double wall = 0;
    for (i = 0; i < 25; i++)
         wall += get_us_dist();
    return (wall/25 < 40 ) ? 0 : 1;
}

int parallel(double *curr_coord){
    int i = 0;
    double leftir;
    double rightir;
    int parallel = 0;
    double error = 0.04 ;
    double time = 30.0;
    set_ir_angle(LEFT, 45);
    set_ir_angle(RIGHT, -45);
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
            position_tracker(curr_coord);
            for ( i = 0; i < time; i++){
                leftir += get_front_ir_dist(LEFT);
                rightir += get_front_ir_dist(RIGHT);
            }
            leftir = leftir/time;
            rightir = rightir/time;
        }   
        else if(rightir/time > leftir/time + error){
            set_motors(-1, 1);
            position_tracker(curr_coord);
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
        printf(" ### Paralleling LEFTIR %f, RIGHTIR %f \n", leftir/time, rightir/time);
        usleep(300000);
        set_motors(0, 0);
        position_tracker(curr_coord);

    }
    set_ir_angle(LEFT, -45);
    set_ir_angle(RIGHT, 45);
    usleep(20000);
}
void centering(){
    set_motors(-14, -14);
    sleep(3);
    reset_motor_encoders();
}