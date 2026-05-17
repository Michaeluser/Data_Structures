#ifndef HASH_TABLE_H
#define HASH_TABLE_H

void delete_HT(double value);
void insert_HT(double value);
double search_HT(double value);

extern long TABLE_SIZE;
extern double* hash_table;
extern char* status_table;

void init_HT();
void free_HT();
unsigned long hash_dtk(double value);
long hash_dp(double value);
void resize_HT();



#endif