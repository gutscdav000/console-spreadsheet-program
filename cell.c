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
    // process_formula(cell->input, cell->output);
    printf("it's a formula\n");
    return "0";
  }
  /////////////////////////////////////////
  // check for text???
  
  printf("didn't match\n");
  return "0";
}

char* process_formula(char* formula, char *ret) {
  int operand1, operand2, op, formPtr = 0, buffPtr = 0;
  char buff[8];
  
  
  
  // read until operation or null terminator
  while((op = op_check(formula[formPtr])) == 0) {
    buff[buffPtr] = formula[formPtr];
    buffPtr++;
    formPtr++;
  }
  buff[buffPtr] = '\0';
  // store operand1
  operand1 = process_operand(buff);
  // if null terminator has been reached
  if(op == 6) {
    sprintf(ret, "%d", operand1);
    return ret;
  }
  // if '(' : 5 recur

  //clear buffer
  memset(buff, '\0', buffPtr);
  buffPtr = 0;

  // read until operation or null terminator
  while((op = op_check(formula[formPtr])) == 0) {
    buff[buffPtr] = formula[formPtr];
    buffPtr++;
    formPtr++;
  }
  // store operand2
  operand2 = process_operand(buff);
  

}

int op_check(char c) {

  switch(c) {
  case '+':
  return 1;
  break;
  case '-':
    return 2;
    break;
  case '*':
    return 3;
    break;
  case '/':
    return 4;
    break;
  case '(':
    return 5;
    break;
    
  case '\0':
    return 6;
    break;
    
  default:
    return 0;
    break;
  }
}

int process_operand(char* c) {
  // is digit
  if(regex_match(c, "[0-9]+") == 1) {
    return atoi(c);
  }
  // is cell reference
  if(regex_match(c, "[A-Z][0-9]+") == 1) {

    //process_cell() to get reference value
    
  }
  //is text
  //if(regex_match(c, "[a-zA-Z]+") == 1)
}


int regex_match(const char *str, const char *pattern) {

  regex_t re;
  int ret;

  // check compilation for errors
  if(regcomp(&re, pattern,  REG_EXTENDED) != 0)
    return 0;

  ret = regexec(&re, str, (size_t)0, NULL, 0);

  if(ret == 0)
    return 1; // match
  else
    return 0; // no match
  
}
