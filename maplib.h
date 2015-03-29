struct node* nodes[17];


void initialize_maze();
int available_adjacent(struct node* node);
int node_in_front(double angle, struct node* currentnode);
int node_on_left(double angle, struct node* currentnode);
int node_on_right(double angle, struct node* currentnode);
void move_to_node(double curr_coord[2],struct node* node);
void map(struct node* currentnode);