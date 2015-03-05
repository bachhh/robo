#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "picomms.h"
#include <math.h> // Compile with -lm flag.

#define LEFT 0
#define RIGHT 1
#define WIDTH 359.00 /* Millimeters, vary on individual robot*/
#define R_WHEEL 76   /* Millimeters, vary on individual robot*/

/*TODO: 
    #2 : motor speed change should occur gradually. 
    #3 : the robot doesn't stop at sharp corner.
    #4 : follow wall algorithm need improvement
    #5 : fix magic numbers on motoroffset()

*/
    

int motor_dynamic(int speed_max,int encoder, double target){
    int speed = 0;
    // Deccelerate for the last 1/10 of the trip
    if ( encoder > (target * 4) / 5 ) {
        speed = ceil( -(4*speed_max/target)*(encoder/4) + speed_max );
    }
    else speed = speed_max;    
    if (speed < 1) speed = 1;  // Avoid some errors
    return speed;
}
/*
void fix_turn(int angle, int side)
{ 
    while(angle_change( leftenc, rightenc, robot_width) > to_rad(angle) ) {
        get_motor_encoders(&leftenc, &rightenc);
        printf("Left Encoder: %d Right Encoder: %d", leftenc, rightenc);
        printf("Face Angle: %f \n", to_degree(angle_change( leftenc, rightenc, robot_width)));
        set_motors(-2 + 2*side*2, 2 - 2*side*2);
    }
    set_motors(0, 0);
}*/

void fix_straight(int target){
    int speed = 0;
    int leftencoder, rightencoder;
    get_motor_encoders( &leftencoder, &rightencoder);
    while (1) {
        get_motor_encoders( &leftencoder, &rightencoder); 
        speed = motor_dynamic(100, leftencoder, target );       
        if (leftencoder >= target){
            set_motors(0,0);
            break;
        }
        set_motors(speed,speed);
    } 
    reset_motor_encoders();
}

void spin(int side, int speed){
    switch(side){
        case LEFT: set_motors(speed, -speed);
        case RIGHT: set_motors(-speed, speed);
    }
}

int turn_cnt(int side, int speed){
    int angle = 0;
    return angle;
}

int straight_cnt(int speed){
    int distance = 0;
    return distance;
}


int motoroffset(int leftIR, int sideIR)
{
    /* Read IR side reading, return the ammount of offset.
         "Offset" is the difference speed between left and right motor
       so that it steer accordingly to sensor
        
        All the "Magic numbers" here work for left front IR at 45 angle,
        and the robot follow with about 20cm distance to the wall.
        Re caculate or make your own function if you want to change those settings.
    */
    
    if ( leftIR < 31 ) return leftIR - 36;
    else if ( leftIR > 39 && leftIR < 45 ) return leftIR - 36;
    else if ( leftIR > 47)     // On sharp corner that steer left,  ( left FRONTIR get reading > 50 )
    {                          // the robot should be driving with its left SIDEIR sensor.     
        if (sideIR <= 20)
            return sideIR - 18;
        else if (sideIR > 20 && sideIR < 40) 
            return (sideIR - 18) * 2;
        else return 0;      // If side IR picks up no obstacle then drive forward
    }
    else return 0;
} 

int speedlimit( int limit, int n) { 
    if (n < 0) return 0; 
    else if ( n > limit) return limit; 
    else return n;
}


double to_rad(double degree)
{
    return degree* (M_PI/180);
}
double to_degree(double rad)
{
    return rad* (180/M_PI);
}
double enc_to_dist(double enc)
{
    // Just a rough estimation based on observing the simulator
    // Wheel diameter = 76mm
    double radius = 76; // millimeters
    // 1 enc = 1 degree turn
    // > 2*M_PI*76 per 360 enc
    return (enc * ((2*M_PI*R_WHEEL)/360));    
}

double angle_change(int* enc)
{
    /*
        Take a pointer to encoder array, return the angle change between previous movement
       ### Positive angle change denote a clockwise turn, and negative is anti clock-wise
       ### Unit is radians
    */
    int lenc, renc;
    get_motor_encoders(&lenc, &renc);
    double ldist = enc_to_dist(enc[0] - lenc);
    double rdist = enc_to_dist(enc[1] - renc);
    return (ldist - rdist) / WIDTH;
}


double position_tracker(int* prevpos, double*xy , double *angle)
{
    //This tracker take pointer to these values: encoder reading, position and angle of robot.
    // Variable prevpos must be pointer to an array of int formatted as : {leftencoder, rightencoder}
    
    int lenc, renc;
    get_motor_encoders(&lenc, &renc);
    double dl = enc_to_dist( lenc - prevpos[0]);
    double dr = enc_to_dist( renc - prevpos[1]);
    
    return 0;
}

int main() {
    connect_to_robot();
    initialize_robot();
                                
    set_ir_angle(RIGHT, -45); // Set right FRONTIR to look foward.
    int leftenc= 0;
    int rightenc= 0;
    get_motor_encoders(&leftenc, &rightenc);
    int face_angle = 0;
    int speed = 20;
    int encoders[2] = {0, 0};  //FORMAT: {leftencoder, rightencoder}
    while(1)
    {
        int frontUS = get_us_dist();
        int rightIR = get_front_ir_dist(RIGHT);  
        int offset = motoroffset( get_front_ir_dist(LEFT), get_side_ir_dist(LEFT) );
        int leftspeed = speedlimit(40, speed - offset);  //It is not obvious that: for IR < 36 left motorgoes
        int rightspeed = speedlimit(40, speed + offset); // faster than right, making it steer to the right.
        
        if ( frontUS > 10 && rightIR > 25) { 
            set_motors(leftspeed, rightspeed);
            printf("Left Encoder: %d Right Encoder: %d ", encoders[0], encoders[1]);
            face_angle += angle_change(encoders);
            get_motor_encoders(encoders, encoders + 1);
            printf("Face Angle: %f \n", to_degree(face_angle) );
        }
        else { 
            set_motors(0, 0); 
            break;
        }  
    }
    return 0;
}


