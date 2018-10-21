#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "cell.h"

void load_file(FILE *in, int x, int y, cell_t *c[x][y]);
void get_dimensions(FILE *in, int* dimesnions);

