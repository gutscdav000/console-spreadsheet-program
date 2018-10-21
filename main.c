#include "load.h"


// *** NOTE ***
// website for parenthesis algorithm:
// faculty.cs.niu.edu/~hutchins/csci241/eval.htmxs
int main(int argc, char *argv[]) {


  char inName[] = "tests2/test1.txt";
  FILE *in = fopen(inName, "r");
  int dims[2];
  
  if(in != NULL) {
  // get dimensions ( dims: [rows, cols] )
  get_dimensions(in, dims);
  fclose(in);
  }
  else {
    printf("ERROR: the file didn't open!\n");
    return -1;
  }
  
  //subtracting 1 from rows because of final line breaks
  // remove if there are no line breaks on last line fo sheet
  int rows = dims[0] - 1, cols = dims[1]; 
  printf("\nrow: %d, col: %d\n", rows, cols);
  
  //create table sheet
  cell_t *table[rows][cols];
  // allocate memory
  for(int i = 0; i <= rows; i++)
    for(int j = 0; j <= cols; j++)
      table[i][j] = malloc(sizeof(cell_t));
  
  
  FILE *inp = fopen(inName, "r");

  if(inp != NULL) {
    printf("loading. . .\n");
    load_file( inp, rows, cols, table);
    fclose(inp);
  }
  else {
    printf("ERROR: the file didn't open!\n");
    return -1;
  }
  
  return 0;
}
