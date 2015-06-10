struct node{
    int name;
    double x, y;
    struct node* adjacent[4];
    int visited;
    struct node* parent;
    struct node* child;
    int discovered;
};

struct point{
	double x, y;
	struct point* next;
};

void initialize_maze();
int available_adjacent(struct node* node);
int node_in_front(double angle, struct node* currentnode);
int node_on_left(double angle, struct node* currentnode);
int node_on_right(double angle, struct node* currentnode);
void move_to_node(double curr_coord[2],struct node* node);
void return_to_node(double curr_coord[2], struct node* returnnode);
void map(double curr_coord[2], struct node* currentnode);
void breadthFirstSearch(struct node* startnode);
void reversePath(struct node* node);
void printPath(struct node* node);
struct point* connect_node(struct point* tail,struct node* from, struct node* to);
void build_path(struct point* tail, struct node* startnode);
void maze_race(double curr_coord[2], struct point* startpoint);