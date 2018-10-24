#include "load.h"

int main() {

  // test regex_match
  char str1[] = "A5";
  char str2[] = "A500";  
  char str3[] = "Z0";
  char str4[] = "hello";
  char str5[] = "1520";
  
  char pattern[] = "[A-Z][0-9]+";
  char pattern1[] = "[A-Za-z]+";
  char pattern2[] = "[0-9]+";
  
  printf("regex test 1 %s\n", regex_match(str1, pattern) == 1? "PASSED": "FAILED");
  printf("regex test 2 %s\n", regex_match(str2, pattern) == 1? "PASSED": "FAILED");
  printf("regex test 3 %s\n", regex_match(str3, pattern) == 1? "PASSED": "FAILED");
  // no match
  printf("regex test 4 %s\n", regex_match(str4, pattern) == 0? "PASSED": "FAILED");
  printf("regex test 5 %s\n", regex_match(str4, pattern1) == 1? "PASSED": "FAILED");
  // no match
  printf("regex test 6 %s\n", regex_match(str2, pattern1) == 0? "PASSED": "FAILED");
  printf("regex test 7 %s\n", regex_match(str5, pattern2) == 1? "PASSED": "FAILED");
  // no match
  printf("regex test 8 %s\n", regex_match(str4, pattern2) == 0? "PASSED": "FAILED");
  printf("regex test 9 %s\n", regex_match("1", pattern2) == 1? "PASSED" : "FAILED");


  // test process formula & process cell

  char inName[] = "tests/test0.txt";
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
      printf("output? %d \t %s\n", table[i][j]->hasOutput, table[i][j]->output);
    }
  }

  


  
  return 0;
}