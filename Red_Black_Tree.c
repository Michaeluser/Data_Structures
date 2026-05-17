#include "Red_Black_Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#pragma region Auxiliary_definitions
#define RETURN_TYPE(x) _Generic((x), \
int: "int", \
double: "double", \
float: "float", \
char*: "char*", \
Node_RB*: "Node_RB*", \
default: "unknown")

#pragma endregion Auxiliary_definitions

Node_RB* NIL = NULL;
void init_NIL() {
    if(NIL == NULL){
        NIL = (Node_RB*) malloc(sizeof(Node_RB));
        NIL->color = BLACK;
        NIL->data = 0;  // Arbitrary value; NIL is identified by its pointer.
        NIL->left_node = NIL;
        NIL->right_node = NIL;
        NIL->parent = NIL;
    }
}
void traverse_and_print_dot(Node_RB* node, FILE* file) {
    if (!node || node->data == 0) return; // Skip NIL nodes
    
    fprintf(file, "    \"%.2f\" [color=%s, fontcolor=%s];\n",
            node->data,
            (node->color == RED) ? "red" : "black",
            (node->color == RED) ? "red" : "black");
    
    if (node->left_node) {
        fprintf(file, "    \"%.2f\" -> \"%.2f\";\n",
                node->data, node->left_node->data);
        traverse_and_print_dot(node->left_node, file);
    }
    
    if (node->right_node) {
        fprintf(file, "    \"%.2f\" -> \"%.2f\";\n",
                node->data, node->right_node->data);
        traverse_and_print_dot(node->right_node, file);
    }
}

void export_to_dot(Node_RB* root, FILE* file) {
    if (!root) return;
    
    fprintf(file, "digraph RBTree {\n");
    fprintf(file, "    node [shape=circle];\n");
    
    traverse_and_print_dot(root, file);
    
    fprintf(file, "}\n");
}

void transplant_RB(Node_RB** root, Node_RB* old_node, Node_RB* new_node) {
    if (old_node->parent == NIL) {
        *root = new_node;
    } else if (old_node == old_node->parent->left_node) {
        old_node->parent->left_node = new_node;
    } else {
        old_node->parent->right_node = new_node;
    }

    if (new_node != NULL) {
        new_node->parent = old_node->parent;
    }
}


void rotate_left(Node_RB* parent, Node_RB* child, Node_RB** root) {
    parent->right_node = child->left_node;

    if (child->left_node != NIL) {
        child->left_node->parent = parent;
    }

    child->parent = parent->parent;

    if (parent->parent == NIL) {
        *root = child;
    } else if (parent == parent->parent->left_node) {
        parent->parent->left_node = child;
    } else {
        parent->parent->right_node = child;
    }

    child->left_node = parent;
    parent->parent = child;
}

void rotate_right(Node_RB* parent, Node_RB* child, Node_RB** root) {
    parent->left_node = child->right_node;

    if (child->right_node != NIL) {
        child->right_node->parent = parent;
    }

    child->parent = parent->parent;

    if (parent->parent == NIL) {
        *root = child;
    } else if (parent == parent->parent->right_node) {
        parent->parent->right_node = child;
    } else {
        parent->parent->left_node = child;
    }

    child->right_node = parent;
    parent->parent = child;
}


void insert_RB(Node_RB** root, double data) {
    // Create a new node and initialize its values
    Node_RB* new_node = (Node_RB*) malloc(sizeof(Node_RB));
    new_node->data = data;
    new_node->color = RED;  // New node is always red
    new_node->left_node = NIL;
    new_node->right_node = NIL;
    new_node->parent = NIL;

    // If the tree is empty, make the new node the root and set its color to black
    if (*root == NULL) {
        *root = new_node;
        new_node->color = BLACK;
        return;
    }

    // Standard Binary Search Tree (BST) insertion
    Node_RB* parent_node = NIL;
    Node_RB* current_node = *root;
    while (current_node != NIL) {
        parent_node = current_node;
        if (data < current_node->data)
            current_node = current_node->left_node;
        else if (data > current_node->data)
            current_node = current_node->right_node;
        else {  // Duplicate value found, no insertion
            free(new_node);
            return;
        }
    }
    new_node->parent = parent_node;
    if (parent_node == NIL) {
        *root = new_node;  // Tree was empty; new node becomes root
    } else if (data < parent_node->data) {
        parent_node->left_node = new_node;
    } else {
        parent_node->right_node = new_node;
    }

    // Fix-up: maintain Red-Black properties
    while (new_node != *root && new_node->parent->color == RED) {
        if (new_node->parent == new_node->parent->parent->left_node) {
            Node_RB* uncle = new_node->parent->parent->right_node;
            if (uncle->color == RED) {
                // Case 1: Uncle is red
                new_node->parent->color = BLACK;
                uncle->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            } else {
                // Case 2: Uncle is black
                if (new_node == new_node->parent->right_node) {
                    new_node = new_node->parent;
                    rotate_left(new_node, new_node->right_node, root);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rotate_right(new_node->parent->parent, new_node->parent->parent->left_node, root);
            }
        } else {
            // Mirror case: parent is on the right side
            Node_RB* uncle = new_node->parent->parent->left_node;
            if (uncle->color == RED) {
                // Case 1: Uncle is red
                new_node->parent->color = BLACK;
                uncle->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            } else {
                // Case 2: Uncle is black
                if (new_node == new_node->parent->left_node) {
                    new_node = new_node->parent;
                    rotate_right(new_node, new_node->left_node, root);
                }
                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                rotate_left(new_node->parent->parent, new_node->parent->parent->right_node, root);
            }
        }
    }

    // Root is always black
    (*root)->color = BLACK;
}


Node_RB* search_RB(Node_RB* root, double searched_data){
    if(RETURN_TYPE(searched_data) != "double" || RETURN_TYPE(root) != "Node_RB*"){printf("Unsupported data type or corrupted root pointer");assert(0);}
    Node_RB* l_root = root;
    while(l_root) 
        if(searched_data > l_root->data){l_root = l_root->right_node;}

        else if(searched_data < l_root->data){l_root = l_root->left_node;}
        
        else{return l_root;}
}

void delete_RB(Node_RB** root, double key) {
    // Find the node to delete
    Node_RB* target_node = *root;
    while (target_node != NIL) {
        if (key == target_node->data)
        {break;}
        else if (key < target_node->data)
        {target_node = target_node->left_node;}
        else{target_node = target_node->right_node;}
    }

    // Key not found, nothing to delete
    if (target_node == NIL)
        return;

    Node_RB* node_to_delete = target_node;
    Node_RB* child_node;
    Color original_color = node_to_delete->color;

    // Case 1: Node has no left child
    if (node_to_delete->left_node == NIL) {
        child_node = node_to_delete->right_node;
        transplant_RB(root, node_to_delete, node_to_delete->right_node);
    }
    // Case 2: Node has no right child
    else if (node_to_delete->right_node == NIL) {
        child_node = node_to_delete->left_node;
        transplant_RB(root, node_to_delete, node_to_delete->left_node);
    }
    // Case 3: Node has two children
    else {
        node_to_delete = target_node->right_node;
        while (node_to_delete->left_node != NIL)
            node_to_delete = node_to_delete->left_node;

        original_color = node_to_delete->color;
        child_node = node_to_delete->right_node;

        // Ensure the right child of the node to be replaced is linked correctly
        if (node_to_delete->parent == target_node)
            child_node->parent = node_to_delete;
        else {
            transplant_RB(root, node_to_delete, node_to_delete->right_node);
            node_to_delete->right_node = target_node->right_node;
            node_to_delete->right_node->parent = node_to_delete;
        }

        transplant_RB(root, target_node, node_to_delete);
        node_to_delete->left_node = target_node->left_node;
        node_to_delete->left_node->parent = node_to_delete;
        node_to_delete->color = target_node->color;
    }

    // Free the memory allocated for the deleted node
    free(target_node);

    // If the deleted node was black, we need to fix the tree to maintain Red-Black properties
    if (original_color == BLACK) {
        while (child_node != *root && child_node->color == BLACK) {
            if (child_node == child_node->parent->left_node) {
                Node_RB* sibling = child_node->parent->right_node;

                // Case 1: Sibling is red
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    child_node->parent->color = RED;
                    rotate_left(child_node->parent, child_node->parent->right_node, root);
                    sibling = child_node->parent->right_node;
                }

                // Case 2: Sibling's children are both black
                if (sibling->left_node->color == BLACK && sibling->right_node->color == BLACK) {
                    sibling->color = RED;
                    child_node = child_node->parent;
                } else {
                    // Case 3: Sibling's right child is black
                    if (sibling->right_node->color == BLACK) {
                        sibling->left_node->color = BLACK;
                        sibling->color = RED;
                        rotate_right(sibling, sibling->left_node, root);
                        sibling = child_node->parent->right_node;
                    }

                    sibling->color = child_node->parent->color;
                    child_node->parent->color = BLACK;
                    sibling->right_node->color = BLACK;
                    rotate_left(child_node->parent, child_node->parent->right_node, root);
                    child_node = *root;
                }

            } else {  // Symmetric case for the right child
                Node_RB* sibling = child_node->parent->left_node;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    child_node->parent->color = RED;
                    rotate_right(child_node->parent, child_node->parent->left_node, root);
                    sibling = child_node->parent->left_node;
                }

                if (sibling->right_node->color == BLACK && sibling->left_node->color == BLACK) {
                    sibling->color = RED;
                    child_node = child_node->parent;
                } else {
                    if (sibling->left_node->color == BLACK) {
                        sibling->right_node->color = BLACK;
                        sibling->color = RED;
                        rotate_left(sibling, sibling->right_node, root);
                        sibling = child_node->parent->left_node;
                    }

                    sibling->color = child_node->parent->color;
                    child_node->parent->color = BLACK;
                    sibling->left_node->color = BLACK;
                    rotate_right(child_node->parent, child_node->parent->left_node, root);
                    child_node = *root;
                }
            }
        }
        child_node->color = BLACK;
    }
}

int main(int argc, char *argv[]){
    FILE* RBT_DELETE_DATA = fopen("RBT_DELETE_DATA.txt", "a+");
    FILE* RBT_SEARCH_DATA = fopen("RBT_SEARCH_DATA.txt", "a+");
    FILE* RBT_INSERT_DATA = fopen("RBT_INSERT_DATA.txt", "a+");
    init_NIL();
    Node_RB* root = NULL;

    if (!RBT_INSERT_DATA || !RBT_SEARCH_DATA || !RBT_DELETE_DATA) {
        perror("Error opening files");
        abort();
    }

    int num_values = 1000000;
    double* array_of_values = (double*)malloc(sizeof(double) * num_values);
    if (!array_of_values) {
        perror("Memory allocation failed");
        return 1;
    }

    srand(time(NULL));
    for (int t = 0; t < num_values; t++) {
        array_of_values[t] = (double)(rand() * rand() % RAND_MAX) / (double)(RAND_MAX * 100) * 80000;
    }

    int i, j;
    int iteration_number;
    int operation_aggregate;
    clock_t start_time, end_time;
    double total_time, time_per_op, ops_per_time;

    for (int attempt = 1; attempt <= 3; attempt++) {
        printf("Attempt %d - Start\n", attempt);

        //_________________RBT_TESTER_INSERT_________________
        fprintf(RBT_INSERT_DATA, "--- Insertion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();
        printf("Debug\n");
        for (j = 0; j < 3; j++) {
            for (i = 0; i < iteration_number; i++) {
                insert_RB(&root, array_of_values[i]);
            }
            printf("Number of iterations - %d\n", iteration_number);
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }

        end_time = clock();
        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(RBT_INSERT_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(RBT_INSERT_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(RBT_INSERT_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(RBT_INSERT_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(RBT_INSERT_DATA, "----------------------------------\n");

         //_________________RBT_TESTER_SEARCH_________________
        fprintf(RBT_SEARCH_DATA, "--- Search Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < 3; j++) {
            for (i = 0; i < iteration_number; i++) {
                search_RB(root, array_of_values[i]);
            }
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }

        end_time = clock();
        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(RBT_SEARCH_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(RBT_SEARCH_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(RBT_SEARCH_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(RBT_SEARCH_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(RBT_SEARCH_DATA, "----------------------------------\n");

        //_________________RBT_TESTER_DELETE_________________
        fprintf(RBT_DELETE_DATA, "--- Deletion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < 3; j++) {
            for (i = 0; i < iteration_number; i++) {
                delete_RB(&root, array_of_values[i]);
            }
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }

        end_time = clock();
        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(RBT_DELETE_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(RBT_DELETE_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(RBT_DELETE_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(RBT_DELETE_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(RBT_DELETE_DATA, "----------------------------------\n");

        printf("Attempt %d - End\n", attempt);
    }
    fclose(RBT_INSERT_DATA);
    fclose(RBT_SEARCH_DATA);
    fclose(RBT_DELETE_DATA);
    return 0;
}


// FILE* visualization = fopen("visualization_redblack.txt", "a+");
// Node_RB* root = NULL;
// init_NIL();
// insert_RB(&root, 44.64);
// insert_RB(&root, 44.24);
// insert_RB(&root, 44.14);
// insert_RB(&root, 44.74);
// insert_RB(&root, 44.344);
// insert_RB(&root, 44.94);
// insert_RB(&root, 45.4);
// insert_RB(&root, 46.64);
// insert_RB(&root, 46.44);
// insert_RB(&root, 46.54);
// insert_RB(&root, 46.84);
// insert_RB(&root, 46.654);
// insert_RB(&root, 46.64);

// printf("%d\n", 44.94 == search_RB(root, 44.94)->data);
// printf("%d\n", 45.4 == search_RB(root, 45.4)->data);
// printf("%d\n", 46.44 == search_RB(root, 46.44)->data);
// printf("%d\n", 46.54 == search_RB(root, 46.54)->data);
// printf("%d\n", 46.84 == search_RB(root, 46.84)->data);
// printf("%d\n", 46.654 == search_RB(root, 46.654)->data);
// printf("%d\n", 46.64 == search_RB(root, 46.64)->data);

// delete_RB(&root, 44.74);
// delete_RB(&root, 44.344);
// delete_RB(&root, 44.94);
// delete_RB(&root, 45.4);
// delete_RB(&root, 46.64);
// delete_RB(&root, 46.44);
// delete_RB(&root, 46.54);
// delete_RB(&root, 46.84);
// delete_RB(&root, 46.654);
// delete_RB(&root, 46.64);
// export_to_dot(root, visualization);