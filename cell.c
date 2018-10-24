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
    process_formula(cell, row_dim, col_dim, c);
    printf("it's a formula\n");
    return "0";
  }
  /////////////////////////////////////////
  // check for text???
  
  printf("didn't match\n");
  return "0";
}

char* process_formula(cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim]) {
  int answer, operand2, op, formPtr = 1, buffPtr = 0;
  char buff[10];
  char operand_buff[10];
  
  
  // read until operation or null terminator
  while((op = op_check(cell->input[formPtr])) == 0) {
    buff[buffPtr] = cell->input[formPtr];
    buffPtr++;
    formPtr++;
  }
  formPtr++;
  buff[buffPtr++] = '\0';

  // store operand1
  process_operand(buff, cell, row_dim, col_dim, c);
  if(strcmp("#NAN", buff) == 0) {
    return "#NAN";
  }
  else if(strcmp("#ERROR", buff) == 0) {
    return "#ERROR";
  }
  else {
    answer = atoi(buff);
  }

  // if null terminator has been reached
  if(op == 6) {
    sprintf(cell->output, "%d", answer);
    cell->hasOutput = 1;
    return cell->output;
  }
  // if '(' : 5 recur

  //clear buffer
  memset(buff, '\0', buffPtr);
  buffPtr = 0;
  //clear buffer
  memset(buff, '\0', 10);

  while( op != 6) {
    // read until operation or null terminator
    while((op = op_check(cell->input[formPtr])) == 0) {
      buff[buffPtr] = cell->input[formPtr];
      buffPtr++;
      formPtr++;
    }
    formPtr++;
    buff[buffPtr++] = '\0';
    
    // store operand2
    //buff =
    process_operand(buff, cell, row_dim, col_dim, c);
    if(strcmp("#NAN", buff) == 0) {
    return "#NAN";
  }
    else if(strcmp("#ERROR", buff) == 0) {
    return "#ERROR";
    }
    else {
      operand2 = atoi(buff);
    }
    
    // if '(' : 5 recur
    
    //clear buffer
    memset(buff, '\0', buffPtr);
    buffPtr = 0;
    memset(operand_buff, '\0', 10);
    
    // answer = operand1 op operand2
    // if next op \0 return ans else return to second while
    answer = process_op(op, operand2, answer);
  }

  sprintf(cell->output, "%d", answer);
  cell->hasOutput = 1;
  return cell->output;
  
}

int process_op(int op, int operand, int answer) {
  switch(op) {
  case 1:
    return operand + answer;
    break;
  case 2:
    return answer - operand;
    break;
  case 3:
    return answer * operand;
    break;
  case 4:
    return answer / operand;
    break;
  default:
    break;
  }
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

char* process_operand(char* buffer, cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim]) {

  // is cell reference
  if(regex_match(buffer, "[A-Z][0-9]+") == 1) {
    // get letter
    int row = char_to_row(buffer[0]);
    // get number
    char temp[8];
    int ptr = 1;
    while(buffer[ptr] != '\0') {
      temp[ptr] = buffer[ptr];
      ptr++;
    }
    int col = atoi(temp);
    //clear buff
    memset(temp, '\0', ptr);
    
    //process cell to get reference value
    // should be a char* must convert to number
    //buffer =
    process_cell(c[row][col], row_dim, col_dim, c);
    return buffer;
  }
  // is digit
  if(regex_match(buffer, "[0-9]+") == 1) {
    return buffer;
  }
  //is text
  //if(regex_match(c, "[a-zA-Z]+") == 1)
}


int regex_match(char *str, char *pattern) {

  regex_t re;
  int ret;

  // check compilation for errors
  if(regcomp(&re, ((const char *)pattern),  REG_EXTENDED) != 0)
    return 0;

  ret = regexec(&re, ((const char*)str), (size_t)0, NULL, 0);

  if(ret == 0)
    return 1; // match
  else
    return 0; // no match
  
}
