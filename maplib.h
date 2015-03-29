struct node{
    int name;
    double x, y;
    struct node* adjacent[4];
    int visited;
    int start;
    int final;
    struct node* parent;
    int discovered;
};

void initialize_maze();
int available_adjacent(struct node* node);
int node_in_front(double angle, struct node* currentnode);
int node_on_left(double angle, struct node* currentnode);
int node_on_right(double angle, struct node* currentnode);
void move_to_node(double curr_coord[2],struct node* node);
void return_to_node(double curr_coord[2], struct node* returnnode);
void map(struct node* currentnode);