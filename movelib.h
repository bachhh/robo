#define R_WHEEL 4.7548

extern double face_angle = 0;

// function prototypes for movelib.c

double to_rad(double degree);
double to_degree(double rad);
double enc_to_dist(int enc);
double position_tracker(double curr_coord[2]);
void spin(double curr_coord[2], double angle);
void go_straight(double curr_coord[2],double distance);
void move_to(double curr_coord[2], double x, double y);
int no_wall_left();
int no_wall_right();
int no_wall_front();
void centering();
