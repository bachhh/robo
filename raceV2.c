//
//  RaceV2.c
//  
//
//  Created by Yee Chong Tan on 16/03/2015.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "picomms.h"


int threshold = 33;
int speed = 45;
int currentLME = 0;
int currentRME = 0;
int a, b = 0;
int flIR;
int frIR;
int lIR;
int rIR;
int squareNumber [16];
int facing = 0;
int sn = 0;

double gain = 20.0;
double width = 22.5;
double initialAngle = 0;
double lastLME = 0;
double lastRME = 0;
double totalX = 0;
double totalY = 0;;
double coordinates[11111][5];


int leftReading() {
    int leftenc, rightenc;
    get_motor_encoders(&leftenc,&rightenc);
    return leftenc;
}

int rightReading() {
    int leftenc, rightenc;
    get_motor_encoders(&leftenc,&rightenc);
    return rightenc;
}

//Math to get angle and distances
void distTraveled() {
    double distance_L;
    double distance_R;
    double angleChange;
    double radius_L;
    double radius_R;
    double radius_M;
    double dx = 0;
    double dy = 0;
    double p = 0;
    double q = 0;
    
    currentLME = leftReading();
    currentRME = rightReading();
    
    double calc_L = ((double)(currentLME-lastLME)/12.1);
    double calc_R = ((double)(currentRME-lastRME)/12.1);
    
    distance_L = calc_L;
    distance_R = calc_R;
    
    angleChange = (distance_L-distance_R)/width;
    
    if (angleChange != 0){
        radius_L = distance_L/angleChange;
        radius_R = distance_R/angleChange;
        radius_M = (radius_L+radius_R)/2;
        
        p = radius_M*(sin(initialAngle+angleChange)-sin(initialAngle));
        q = radius_M*(cos(initialAngle)-cos(initialAngle+angleChange));
        
        dx = q;
        dy = p;
        
        initialAngle = initialAngle + angleChange;
    }
    else {
        dx = distance_R*sin(initialAngle);
        dy = distance_L*cos(initialAngle);
    }
    totalX = totalX + dx;
    totalY = totalY + dy;
    // markSquare();
    lastLME = currentLME;
    lastRME = currentRME;
    /*
    printf("totalX\ttotalY\t");
    printf("%5f\t%5f\n",totalX, totalY);
    printf("initialAngle: ");
    printf("%5f\t\n", initialAngle);
      */
    set_point(totalX, totalY);
    
    //coordinates[b][0] = totalX;
    //coordinates[b][1] = totalY;
    //b++;
}

int markSquare(){
    int pos1;
    int pos2;
    int pos_final;
    
    pos1 = (totalX+30)/60;
    pos2 = (totalY+30)/60;
    if (pos1==0 && pos2==0) {
        pos_final=0;
        squareNumber[sn] = pos_final;
        printf("SquareNumber\t");
        printf("%5i\n",pos_final);
        printf("ArrayNumber\t");
        printf("%5i\n\n",sn);
        sn++;
        
    }
    else {
        pos_final = pos2 + pos1*4;
        squareNumber[sn] = pos_final;
        printf("SquareNumber\t");
        printf("%5i\n",pos_final);
        printf("ArrayNumber\t");
        printf("%5i\n\n",sn);
        sn++;
    }

}



//Basic Moves
void pause()
{
    int i;
    for (i=0; i<111;i++){
        set_motors (0,0);
    }
}

void positioning()
{
    int wall_in_front = get_us_dist();
    if(wall_in_front < 20)
    {
        while(wall_in_front <= 29)
        {
            set_motors(-5,-5);
            wall_in_front = get_us_dist();
            distTraveled();
        }
        pause();
    }
    
    int flIR;
    int frIR;
    get_front_ir_dists(&flIR, &frIR);
        set_ir_angle (RIGHT, -45);
    set_ir_angle (LEFT, 45);
    
    if(flIR != frIR)
    {
        if(flIR > frIR)
        {
            while(flIR != frIR)
            {
                set_motors(2,-2);
                get_front_ir_dists(&flIR, &frIR);
                distTraveled();
            }
        }
        else if(flIR < frIR)
        {
            while(flIR != frIR)
            {
                set_motors(-2,2);
                get_front_ir_dists(&flIR, &frIR);
                distTraveled();
            }
        }
    }
    pause();
}


void turnLeft()
{
    double anAngle = initialAngle;
    while (-M_PI/2 < initialAngle - anAngle) {
        if (initialAngle - anAngle < -M_PI*1/3){
            set_motors (-4,4);
            distTraveled();
        }

        else
        {
            set_motors(-30,30);
            distTraveled();
        }
        /*
        printf("anAngle: ");
        printf("%5f\t\n", anAngle);
        printf("initialAngle - anAngle: ");
        printf("%5f\t\n", initialAngle - anAngle);
         */
    }
    pause();
}


int turnRight()
{
    double anAngle = initialAngle;
    while (initialAngle - anAngle < M_PI/2) {
        if (initialAngle - anAngle > M_PI*1/3){
            set_motors (4,-4);
            distTraveled();
        }
        else
        {
            set_motors(30,-30);
            distTraveled();
        }
    }
    pause();
}
    
    
void moveOneBoxForward()
{
    int i;
    int leftenc, rightenc;
    get_motor_encoders (&leftenc, &rightenc);
    int currentEncValue = leftenc;
    
    while (leftenc - currentEncValue < 687) {
        get_motor_encoders (&leftenc, &rightenc);
        get_front_ir_dists(&flIR, &frIR);
        get_side_ir_dists(&lIR, &rIR);
        if(frIR > 21) {
            set_motors (30,30);
            printf("Charge\n\n");
        }
        else {
            break;
        }
        distTraveled();
    }
    pause();
}

void uTurn()
{
    double anAngle = initialAngle;
    while (initialAngle - anAngle < M_PI) {
        if (initialAngle - anAngle > M_PI/2){
            set_motors (4,-4);
            distTraveled();
        }
        else
        {
            set_motors(10,-10);
            distTraveled();
        }
    }
}


//Movements of the robots
void explore()
{
    while (1) {
        get_front_ir_dists(&flIR, &frIR);
        get_side_ir_dists(&lIR, &rIR);
       /*
        if (lIR < 23 && rIR < 23 && frIR > 23) {
            moveOneBoxForward();
            printf("Charge\n\n");
        }
        */
        if (lIR > 35) {
            turnLeft();
            moveOneBoxForward();
            printf("Left\n\n\n");
            positioning();
            markSquare();
        }
        else if (rIR > 35 && lIR < 35 && frIR < 22) {
            turnRight();
            moveOneBoxForward();
            printf("Right\n\n\n");
            positioning();
            markSquare();
        }
        else if (lIR < 36 && rIR < 36 && frIR < 22) {
            uTurn();
            moveOneBoxForward();
            printf("Retreat\n\n\n");
            positioning();
            markSquare();
        }
        else {
            moveOneBoxForward();
            printf("Roar\n\n");
            positioning();
            markSquare();
            }
        }
}
    
int main ()
{
    connect_to_robot();
    initialize_robot();
    set_origin();
    set_ir_angle (RIGHT, -45);
    
    if (sn < 16) {
        explore();
    }
    else {
        if (-15 < totalX < 15 && -15 < totalY < 15) {
            set_motors(0,0);
            
            int o;
            for (o=0; o < sn; o++) {
                printf("5i",squareNumber[o]);
            }
        }
        else {
            explore();
        }
    }
    
    


    return 0;
}

/*
void find_minimum(int k)
{
    int i;
    if (ok2>k)
    {
        ok2=k;
        for(i=1;i<=k;i++)
        {
            res[i]=X[i];
        }
    }
    //fout<<endl;
}
void back(int k)
{
    int i;
    printf ("%i     %i \n", p[1], A[X[k-1]][1]);
    for(i=1;i<=17;i++)
    {
        printf ("%i     %i \n", p[i], A[X[k-1]][i]);
        if(!p[i] && A[X[k-1]][i]==1)
        {
            X[k]=i;
            p[i]=1;
            if(i==16)
                find_minimum(k);
            else
                back(k+1);
            p[i]=0;
        }
    }
}
void find_shortest_path()
{
    X[1]=0;
    p[0]=1;
    back(2);
    
    int i;
    printf ("\nShortest path\n");
    for (i=1; i<=ok2; i++)
        printf ("  %i  ", res[i]);
    
    printf ("\n\n\n");
}
*/