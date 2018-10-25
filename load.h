#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "cell.h"

void load_file(FILE *in, int x, int y, cell_t *c[x][y]);
void get_dimensions(FILE *in, int* dimesnions);
int push_reference(char *ref, char** ref_cache, int *cachePtr);
int contains(char *ref, char **ref_cache, int *cachePtr);
void clear_cache(char ***ref_cache, int *ptr);
void print_spreadsheet(char *out, int rows, int cols, cell_t *c[rows][cols]);

#define DEFAULT_REF_SIZE 5
#define DEFAULT_CACHE_SIZE 100
