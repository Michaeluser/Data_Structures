#include <stdio.h>
#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

// Node and Color Definition
typedef enum color {
    RED = 1,
    BLACK = 0
} Color;

typedef struct Node_RB {
    Color color;
    double data;
    struct Node_RB* left_node;
    struct Node_RB* right_node;
    struct Node_RB* parent;
} Node_RB;

// Global NIL Node
extern Node_RB* NIL;

// Function Declarations
void init_NIL();
void traverse_and_print_dot(Node_RB* node, FILE* file);
void export_to_dot(Node_RB* root, FILE* file);
void transplant_RB(Node_RB** root, Node_RB* old_node, Node_RB* new_node);
void rotate_left(Node_RB* parent, Node_RB* child, Node_RB** root);
void rotate_right(Node_RB* parent, Node_RB* child, Node_RB** root);
void insert_RB(Node_RB** root, double data);
Node_RB* search_RB(Node_RB* root, double searched_data);
void delete_RB(Node_RB** root, double key);

#endif // RB_TREE_H
