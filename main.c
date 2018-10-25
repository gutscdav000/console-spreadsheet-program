#include "load.h"


int main(int argc, char *argv[]) {

  
  char inName[100];// = "tests/test3.txt";

   if(argc == 2) {
     
     sprintf(inName, "%s", argv[1]);
  }
  else {
    printf("ERROR: please enter the file to be parsed\n");
    return -1;
  }
   


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
    printf("loading %s . . .\n", inName);
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

  int cachePtr = 0;
  char** ref_cache = NULL;
  clear_cache(&ref_cache, &cachePtr);
  
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      printf("%d,%d\n", i, j);
      process_cell(table[i][j], rows, cols, table, ref_cache, &cachePtr);
      clear_cache(&ref_cache, &cachePtr);
      printf("output? %d \t %s\n", table[i][j]->hasOutput, table[i][j]->output);
    }
  }

  if(ref_cache != NULL)
    free(ref_cache);


  
  return 0;

}
