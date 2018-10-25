#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

typedef struct cell {
  char input[150];
  char output[100];
  int hasOutput;
} cell_t;

// col_index -> char & char -> col_index macros
// int -> char & char -> int [65:'A', 90:'Z']
#define col_to_char(i) ((char)(i + 65))
#define char_to_col(c) (((int)(c)) - 65)

// row_index -> char & char -> row_index macros
// int -> char & char -> int [48:'0', 57: '0']
#define row_to_char(i) ((char)(i + 48))
#define char_to_row(c) (((int)c) - 48)

char* process_cell(cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr);
void process_formula(char *formula, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr);
int op_check(char c);
int process_operand(char* buffer, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr);
int regex_match( char *, char *);
int process_op(int op, int operand, int answer);
void get_cell_posn(char *ref, cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim]);
