#include "cell.h"


char* process_cell(cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim]) {

  // check if there is already an output
  if(cell->hasOutput == 1) {
    return cell->output;
  }
  
  // check if it's a digit
  int integer = atoi(cell->input);
  // check for valid input
  if(integer > 0 || integer < 0) {
    cell->hasOutput = 1;
    sprintf(cell->output, "%d", integer);
    printf("%s\n", cell->output);
    return cell->output;
  }
  // validate it's not an error if zero is returned
  else if(integer == 0 && strcmp(cell->input,"0") == 0) {
      cell->hasOutput = 1;
      sprintf(cell->output, "%s", "0");
      printf("%s\n", cell->output);
      return cell->output;
  }// otherwise it was an erronious return 0
  //////////////////////////////////////////
  
  // check if it's an empty cell
  if(strcmp(cell->input,"#NAN") == 0) {
    cell->hasOutput = 1;
    sprintf(cell->output, "%s", "#NAN");
    printf("%s\n", cell->output);
    return cell->output;
  }
  //////////////////////////////////////////
  // check if it's a formula
  if((cell->input)[0] == '=') {
    // call process formula function
    printf("it's a formula\n");
    return "0";
  }
  /////////////////////////////////////////
  // check for text???
  
  printf("didn't match\n");
  return "0";
}
