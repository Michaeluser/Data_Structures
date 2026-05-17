#include "Hash_Table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma region Auxiliary_definitions

#define EMPTY 0 
#define DELETED -1
#define OCCUPIED 1
long TABLE_SIZE = 60013;

double* hash_table = NULL;
char* status_table = NULL;

#pragma endregion Auxiliary_definitions

#pragma region Auxiliary_functions

void test_hash_table(double* array_of_values, int num_values, int num_attempts) {
    FILE* HT_INSERT_DATA = fopen("HT_INSERT_DATA.txt", "w");
    FILE* HT_SEARCH_DATA = fopen("HT_SEARCH_DATA.txt", "w");
    FILE* HT_DELETE_DATA = fopen("HT_DELETE_DATA.txt", "w");

    if (!HT_INSERT_DATA || !HT_SEARCH_DATA || !HT_DELETE_DATA) {
        perror("Error opening files");
        return;
    }

    int i, j;
    int iteration_number;
    int operation_aggregate;
    clock_t start_time, end_time;
    double total_time, time_per_op, ops_per_time;

    for (int attempt = 1; attempt <= num_attempts; attempt++) {
        printf("Attempt %d - Start\n", attempt);

        // Insertion Test
        fprintf(HT_INSERT_DATA, "--- Insertion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < 3; j++) {
            for (i = 0; i < iteration_number; i++) {
                insert_HT(array_of_values[i]);
            }
            printf("Number of iterations - %d\n", iteration_number);
            operation_aggregate += iteration_number;
            iteration_number *= 10;
        }

        end_time = clock();
        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        time_per_op = total_time / operation_aggregate;
        ops_per_time = operation_aggregate / total_time;

        fprintf(HT_INSERT_DATA, "Total Operations: %d\n", operation_aggregate);
        fprintf(HT_INSERT_DATA, "Total Time: %f seconds\n", total_time);
        fprintf(HT_INSERT_DATA, "Time per Operation: %e seconds/op\n", time_per_op);
        fprintf(HT_INSERT_DATA, "Operations per Second: %e ops/sec\n", ops_per_time);
        fprintf(HT_INSERT_DATA, "----------------------------------\n");

        // Search Test
        fprintf(HT_SEARCH_DATA, "--- Search Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < 3; j++) {
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

        // Deletion Test
        fprintf(HT_DELETE_DATA, "--- Deletion Test - Attempt %d ---\n", attempt);
        iteration_number = 1000;
        operation_aggregate = 0;
        start_time = clock();

        for (j = 0; j < 3; j++) {
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
        free(hash_table);
        free(status_table);
        init_HT();
    }

    fclose(HT_INSERT_DATA);
    fclose(HT_SEARCH_DATA);
    fclose(HT_DELETE_DATA);
}

void init_HT(){
    hash_table = (double*) malloc(sizeof(double)* TABLE_SIZE);
    status_table = (char*) malloc(sizeof(char) * TABLE_SIZE);
    memset(status_table, EMPTY, sizeof(char) * TABLE_SIZE);
}

void free_HT(){
    free(hash_table);
    free(status_table);
}

unsigned long hash_dtk(double value) { // means hash data to double
    union {
        double d;
        unsigned long long u;
    } convert;
    convert.d = value;
    return (unsigned long)(convert.u % LONG_MAX); // Ensures it's portable to `long`
}

long hash_dp(double value){// means hash dynamic polynomial
    static int generate = 1;
    
    int maximum_coefficient_amount = 40;
    static long random_values_list[40];
    if(generate){
        for(int i = 0; i < maximum_coefficient_amount; i++){random_values_list[i] = rand() % LONG_MAX;}
        generate = 0;
    }
    long output = 0;
    long key = hash_dtk(value);
    for(int i = 0; i < maximum_coefficient_amount; i++){
        output += (long)((random_values_list[i]* (key % (i + 2))) % LONG_MAX);
    }
    return output % LONG_MAX;
}

void resize_HT(){
    long OLD_TABLE_SIZE = TABLE_SIZE;
    TABLE_SIZE = (long)(TABLE_SIZE* 4);
    while (1) {
        int is_prime = 1;
        if (TABLE_SIZE <= 1) {
            is_prime = 0;
        } else if (TABLE_SIZE <= 3) {
            is_prime = 1;
        } else if (TABLE_SIZE % 2 == 0 || TABLE_SIZE % 3 == 0) {
            is_prime = 0;
        } else {
            for (long i = 5; i * i <= TABLE_SIZE; i += 6) {
                if (TABLE_SIZE % i == 0 || TABLE_SIZE % (i + 2) == 0) {
                    is_prime = 0;
                    break;
                }
            }
        }
    
        if (is_prime) break;
        TABLE_SIZE++;
    }

    double* new_table = (double*) malloc(sizeof(double)*TABLE_SIZE);
    char* new_status_table = (char*) malloc(sizeof(char)*TABLE_SIZE);
    memset(new_status_table, EMPTY, sizeof(char) * TABLE_SIZE);

    
    if(hash_table != NULL && status_table != NULL){
        for(int i = 0; i < OLD_TABLE_SIZE; i++){
            if(status_table[i] == OCCUPIED){

                long probe_number;
                long key;
                double value = hash_table[i];
                for(probe_number = 0; probe_number < TABLE_SIZE; probe_number++){
                    key = (hash_dtk(value) + probe_number * hash_dp(value)) % TABLE_SIZE;
                    if(new_table[key] != value){
                        new_table[key] = value; 
                        new_status_table[key] = OCCUPIED; 
                        break;
                    }
                }

            }
        }
    }
    free(hash_table);
    free(status_table);
    
    hash_table = NULL;
    status_table = NULL;

    hash_table = new_table;
    status_table = new_status_table;

}

#pragma endregion Auxiliary_functions

#pragma region Insert_function
void insert_HT(double value){
    static long occupied = 0;
    long probe_number;
    long index;
    for(long probe_number = 0; probe_number < TABLE_SIZE; probe_number++){
        index = (hash_dtk(value) + probe_number * hash_dp(value)) % TABLE_SIZE;
        if(hash_table[index] != value && status_table[index] != OCCUPIED){
            hash_table[index] = value; 
            status_table[index] = OCCUPIED;
            occupied++; 
            break;
        }
    }
    

    float load_factor = (float)occupied / TABLE_SIZE;
    if (load_factor > 0.8) {resize_HT();}

    
}
#pragma endregion Insert_function

#pragma region Search_function
double search_HT(double value){
    long probe_number;
    long index;
    for(probe_number = 0; probe_number < TABLE_SIZE; probe_number++){
        index = (hash_dtk(value) + probe_number * hash_dp(value)) % TABLE_SIZE;
        if(hash_table[index] == value && status_table[index] == OCCUPIED){return hash_table[index];}
    }
    return -1.00;
}
#pragma endregion Search_function

#pragma region Delete_function
void delete_HT(double value){
    long probe_number;
    long index;
    for(long probe_number = 0; probe_number < TABLE_SIZE; probe_number++){
        index = (hash_dtk(value) + probe_number * hash_dp(value)) % TABLE_SIZE;
        if(status_table[index] == EMPTY){break;}
        if(hash_table[index] == value && status_table[index] == OCCUPIED){status_table[index] = DELETED; break;}
    }
}
#pragma endregion Delete_function

#pragma region Main_function
int main(){
    init_HT(); // Initialize the hash table

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

    test_hash_table(array_of_values, num_values, 3);

    free(array_of_values);
    free_HT(); // Free the hash table
    return 0;
    }
    

    
#pragma endregion Main_function