#include "load.h"

char* process_cell(cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr) {

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
    char cellRef[DEFAULT_REF_SIZE];
    get_cell_posn(cellRef, cell, row_dim, col_dim, c);
    push_reference(cellRef, ref_cache, cachePtr);
    process_formula(cell, row_dim, col_dim, c, ref_cache, cachePtr);
    printf("it's a formula\n");
    return "0";
  }
  /////////////////////////////////////////
  // check for text???
  
  return "0";
}

char* process_formula(cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr) {
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
  process_operand(buff, cell, row_dim, col_dim, c, ref_cache, cachePtr);
  if(strcmp("#NAN", buff) == 0) {
    cell->hasOutput = 1;
    strcpy(cell->output, "#NAN");
    return "#NAN";
  }
  else if(strcmp("#ERROR", buff) == 0) {
    cell->hasOutput = 1;
    strcpy(cell->output, "#ERROR");
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

  int prev_op;
  while( op != 6) {
    prev_op = op;
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
    process_operand(buff, cell, row_dim, col_dim, c, ref_cache, cachePtr);
    if(strcmp("#NAN", buff) == 0) {
      cell->hasOutput = 1;
      strcpy(cell->output, buff);
      return "#NAN";
    }
    else if(strcmp("#ERROR", buff) == 0) {
      cell->hasOutput = 1;
      strcpy(cell->output, "#ERROR");
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
    answer = process_op(prev_op, operand2, answer);
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

char* process_operand(char* buffer, cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char **ref_cache, int *cachePtr) {

  // is cell reference
  if(regex_match(buffer, "[A-Z][0-9]+") == 1) {
    // get letter
    int col = char_to_col(buffer[0]);
    // get number
    char temp[8];
    int ptr = 1;
    while(buffer[ptr] != '\0') {
      temp[ptr - 1] = buffer[ptr];
      ptr++;
    }
    int row = atoi(temp) - 1; // i 1 because it's an index
    //clear buff
    memset(temp, '\0', ptr);

    // check for cyclical references ***
    if(push_reference(buffer, ref_cache, cachePtr) == 1) {
      strcpy(buffer, "#ERROR");
      return "#ERROR";
    }
    //process cell to get reference value
    process_cell(c[row][col], row_dim, col_dim, c, ref_cache, cachePtr);
    memset(buffer, '\0', ptr);
    strcpy(buffer, c[row][col]->output);
    return c[row][col]->output;
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

void get_cell_posn(char *ref, cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim]) {

  for(int i = 0; i < row_dim; i++) {
    for(int j = 0; j < col_dim; j++) {
      if(cell == c[i][j]) {
	  char let = col_to_char(j);
	  //int num = atoi(i +1);
	  sprintf(ref, "%c%d", let, i + 1);
	  return;
      }
    }
  }
}
