#include "load.h"


int main(void) {

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
  
  // test recursive formula processing
  
  cell_t * temp[5][5];// = malloc(sizeof(cell_t) * 25);
  /*for(int i = 0; i < 5; i++)
    for(int j = 0; j < 5; j++)
      temp[i][j] = malloc(sizeof(cell_t));
  */
  
  char **t;// = malloc(sizeof(char) * 5);
  int* x;
  //*x = 5;
  int ans;

  /*  
char form3[] = "=(11+7)/2";
 process_formula(form3, 1, 1, temp, t, x);
 printf("%s\n", form3);
 ans = atoi(form3);
 printf("recursive formula test %d %s\n", 3, ans == 9? "PASSED" : "FAILED");
  
 char form2[] = "=(11+7)/(8-6)";
 process_formula(form2, 1, 1, temp, t, x);
 printf("%s\n", form2);
 ans = atoi(form2);
printf("recursive formula test %d %s\n", 2,  ans == 9? "PASSED" : "FAILED");
  
 char form1[] = "=5-(6+5)";
 process_formula(form1, 1, 1, temp, t, x);
 printf("%s\n", form1);
 ans = atoi(form1);
 printf("recursive formula test %d %s\n", 1, ans == -6? "PASSED" : "FAILED");
  
  */

  // test text concat

  /*
  char string1[] = "hello+ +world!";
  char string2[] = "Bitte+ +schön";
  char string3[] = "漢字+汉字";
  char string4[] = "=world+ + hello?";

  char ret[150];

  process_formula(string1, 1, 1, temp, t, x);
  printf("string test 1 %s\n", string1);

  process_formula(string2, 1, 1, temp, t, x);
  printf("string test 2 %s\n", string2);

  process_formula(string3, 1, 1, temp, t, x);
  printf("string test 3 %s\n", string3);
  */

  // test process formula & process cell
 
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

  free(ref_cache);


  
  return 0;
}
