#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Red_Black_Tree.h"
// #include "Scape_Goat_Tree.h"
#include "Hash_Table.h"

int main(){
    int num_attempts;
    int num_threshold;

    printf("Enter number of tests:");
    scanf("%d", &num_attempts);
    printf("\n");
    printf("Enter threshold number for each test:");
    scanf("%d", &num_threshold);
    FILE* RBT_DELETE_DATA = fopen("RBT_DELETE_DATA.txt", "a+");
    FILE* RBT_SEARCH_DATA = fopen("RBT_SEARCH_DATA.txt", "a+");
    FILE* RBT_INSERT_DATA = fopen("RBT_INSERT_DATA.txt", "a+");
    FILE* HT_INSERT_DATA = fopen("HT_INSERT_DATA.txt", "a+");
    FILE* HT_SEARCH_DATA = fopen("HT_SEARCH_DATA.txt", "a+");
    FILE* HT_DELETE_DATA = fopen("HT_DELETE_DATA.txt", "a+");


    double *array_of_values = (double*) malloc(sizeof(double) * 1000000);
    srand(time(NULL));
    double rand_number;
    for(int t = 0; t < 1000000; t++){
        rand_number = (double) (rand() * rand() % RAND_MAX);
        array_of_values[t] = (rand_number / (double) (RAND_MAX * 100)) * 80000;// generate 1000000 random floating point numbers under 80000
    }
    init_NIL();
    init_HT();
    Node_RB* root = NULL;


    int i, j;
    int iteration_number;
    int operation_aggregate;
    clock_t start_time, end_time;
    double total_time, time_per_op, ops_per_time;


    for (int attempt = 1; attempt <= num_attempts; attempt++) {
        printf("Attempt %d - Start\n", attempt);

        //_________________HT_TESTER_INSERT_________________
        fprintf(HT_INSERT_DATA, "--- Insertion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();
        for (j = 0; j < num_threshold; j++) {
            for (i = 0; i < iteration_number; i++) {
                insert_HT(array_of_values[i]);
            }
            printf("Number of iterations - %d\n", iteration_number);
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }
        printf("HT Insert done.\n");
        end_time = clock();

        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(HT_INSERT_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(HT_INSERT_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(HT_INSERT_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(HT_INSERT_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(HT_INSERT_DATA, "----------------------------------\n");

        //_________________HT_TESTER_SEARCH_________________
        fprintf(HT_SEARCH_DATA, "--- Search Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < num_threshold; j++) {
            for (i = 0; i < iteration_number; i++) {
                search_HT(array_of_values[i]);
            }
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }

        end_time = clock();
        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(HT_SEARCH_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(HT_SEARCH_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(HT_SEARCH_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(HT_SEARCH_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(HT_SEARCH_DATA, "----------------------------------\n");

        //_________________HT_TESTER_DELETE_________________
        fprintf(HT_DELETE_DATA, "--- Deletion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < num_threshold; j++) {
            for (i = 0; i < iteration_number; i++) {
                delete_HT(array_of_values[i]);
            }
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }

        end_time = clock();
        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(HT_DELETE_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(HT_DELETE_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(HT_DELETE_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(HT_DELETE_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(HT_DELETE_DATA, "----------------------------------\n");

        printf("Attempt %d - End\n", attempt);
        free_HT();
        init_HT();
    }

    for (int attempt = 1; attempt <= num_attempts; attempt++) {
        printf("Attempt %d - Start\n", attempt);

        //_________________RBT_TESTER_INSERT_________________
        fprintf(RBT_INSERT_DATA, "--- Insertion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();
        for (j = 0; j < num_threshold; j++) {
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

        for (j = 0; j < num_threshold; j++) {
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

        for (j = 0; j < num_threshold; j++) {
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
        root = NULL;
        init_NIL();
    }
    
    

    fclose(RBT_INSERT_DATA);
    fclose(RBT_SEARCH_DATA); 
    fclose(RBT_DELETE_DATA); 

    fclose(HT_INSERT_DATA);  
    fclose(HT_SEARCH_DATA);  
    fclose(HT_DELETE_DATA);
    free(array_of_values);
    return 0;
    
}