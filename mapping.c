#include <stdlib.h>
#include <mapping.h>

struct node {
    int x;
    int y;
    struct node* next;
};

struct node* linked_list_initialize(){
    struct node* root = malloc(sizeof(struct node));
    root->x = 0;
    root->y = 0;
    root->next = NULL;
    return root;
}

void add_node(struct node** current, int x, int y){
    struct node* newnode = malloc(sizeof(struct node));
    if (newnode == NULL){    
       printf("Out of memory");    
       exit(0);
    }
    newnode->next = NULL;
    newnode->x = x;
    newnode->y = y;
    *current->next = newnode;
}

int main() {     
    struct node* root = linked_list_initialize();
    struct node* current = root;
    add_node(&current, 10, 10);
}
