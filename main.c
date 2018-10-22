#include "load.h"


// *** NOTE ***
// website for parenthesis algorithm:
// faculty.cs.niu.edu/~hutchins/csci241/eval.htmxs
int main(int argc, char *argv[]) {


  char inName[] = "tests/test3.txt";
  FILE *in = fopen(inName, "r");
  int dims[2];

  // get dimensions ( dims: [rows, cols] )
  if(in != NULL) {
  get_dimensions(in, dims);
  fclose(in);
  }
  else {
    printf("ERROR: the file didn't open!\n");
    return -1;
  }
  
  int rows = dims[0], cols = dims[1]; 
  printf("\nrow: %d, col: %d\n", rows, cols);
  /////////////////////////////////////////////

  
  //create table sheet
  cell_t *table[rows][cols];
  // allocate memory
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      table[i][j] = malloc(sizeof(cell_t));
      table[i][j]->hasOutput = 0;
    }
  }
  //////////////////////////////////////////////


  // load spreadsheet
  in = fopen(inName, "r");

  if(in != NULL) {
    printf("loading. . .\n");
    load_file( in, rows, cols, table);
    fclose(in);
  }
  else {
    printf("ERROR: the file didn't open!\n");
    return -1;
  }
  ///////////////////////////////////////////////

  // process cells
  
  printf("processing cells: row,col\n");
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      printf("%d,%d\n", i, j);
      process_cell(table[i][j], rows, cols, table);
    }
  }

  return 0;
}
