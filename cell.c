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
    return cell->output;
  }
  // validate it's not an error if zero is returned
  else if(integer == 0 && strcmp(cell->input,"0") == 0) {
    cell->hasOutput = 1;
    sprintf(cell->output, "%s", "0");
    return cell->output;
  }// otherwise it was an erronious return 0
  //////////////////////////////////////////
  
  // check if it's an empty cell
  if(strcmp(cell->input,"#NAN") == 0) {
    cell->hasOutput = 1;
    sprintf(cell->output, "%s", "");
    return cell->output;
  }
  //////////////////////////////////////////
  // check if it's a formula
  if((cell->input)[0] == '=') {
    // call process formula function
    char cellRef[DEFAULT_REF_SIZE];
    char formula[150];
    sprintf(formula, "%s", cell->input);
    get_cell_posn(cellRef, cell, row_dim, col_dim, c);
    push_reference(cellRef, ref_cache, cachePtr);
    process_formula(formula, row_dim, col_dim, c, ref_cache, cachePtr);
    sprintf(cell->output, "%s", formula);
    cell->hasOutput = 1;
    return "0";
  }
    
  return "0";
}

void process_formula(char *formula, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr) {
  int answer, operand2, op, formPtr = formula[0] == '='? 1: 0, buffPtr = 0, isText = 0;
  char buff[150];
  char buff2[150];
  
  // read until operation or null terminator
  while((op = op_check(formula[formPtr])) == 0) {
    buff[buffPtr] = formula[formPtr];
    buffPtr++;
    formPtr++;
  }
  
  formPtr++;
  if(op != 5) buff[buffPtr++] = '\0';
  
  
  // if '(' : 5 recur
  if(op == 5) {
    
    while((op = op_check(formula[formPtr])) != 7) {
      buff[buffPtr] = formula[formPtr];
      buffPtr++, formPtr++;
    }
    op = op_check(formula[++formPtr]);
    formPtr++;
    buff[buffPtr++] = '\0';
    
    process_formula(buff, row_dim, col_dim, c, ref_cache, cachePtr);
  }
  
  
  
  // store operand1
  isText = process_operand(buff, row_dim, col_dim, c, ref_cache, cachePtr);
  if(isText == 1) {
    strcpy(buff2, buff);
  }
  
  if(strcmp("#NAN", buff) == 0) {
    memset(formula, '\0', 150); 
    strcpy(formula, "#NAN");
    return;
  }
  else if(strcmp("#ERROR", buff) == 0) {
    memset(formula, '\0', 150);
    strcpy(formula, "#ERROR");
    return;
  }
  else {
    answer = atoi(buff);
  }
  
  // if null terminator has been reached
  if(op == 6) {
    memset(formula, '\0', 150);
    
    if(isText == 1)
      sprintf(formula, "%s", buff);
    else 
      sprintf(formula, "%d", answer);
    return;
  }
  
  buffPtr = 0;
  //clear buffer
  memset(buff, '\0', 150);

  
  

  int prev_op;
  while( op != 6) {
    prev_op = op;
    // read until operation or null terminator
    while((op = op_check(formula[formPtr])) == 0) {
      buff[buffPtr] = formula[formPtr];
      buffPtr++, formPtr++;
    }
    
    formPtr++;
    if(op != 5) buff[buffPtr++] = '\0';

    // recur on open paren: '('
    if(op == 5) {

      // while not close paren: ')'
      while((op = op_check(formula[formPtr])) != 7) {
	buff[buffPtr] = formula[formPtr];
	buffPtr++, formPtr++;
      }
      
      buff[buffPtr++] = '\0';
      op = op_check(formula[++formPtr]);
      
      process_formula(buff, row_dim, col_dim, c, ref_cache, cachePtr);
    }
    
    
    // store operand2
    process_operand(buff, row_dim, col_dim, c, ref_cache, cachePtr);
    if(strcmp("#NAN", buff) == 0) {
      memset(formula, '\0', 150);
      strcpy(formula, buff);
      return;
    }
    else if(strcmp("#ERROR", buff) == 0) {
      memset(formula, '\0', 150);
      strcpy(formula, "#ERROR");
      return;
    }
    else {
      operand2 = atoi(buff);
    }
    
    
    
    if(isText == 1 && prev_op == 1) {
      //copy second operand into first 
      strcat(buff2, buff);
    }
    else {
      // if next op \0 return ans else return to second while
      answer = process_op(prev_op, operand2, answer);
    }
    
    //clear buffer
    memset(buff, '\0', buffPtr);
    buffPtr = 0;

  }
  
  //clear return pointer
  memset(formula, '\0', 150);
  
  if(isText == 1) // for text concatenation
    sprintf(formula, "%s", buff2);
  else //for numeric return types
    sprintf(formula, "%d", answer);
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
  case ')':
    return 7;
    break;
    
  default:
    return 0;
    break;
  }
}


int process_operand(char* buffer, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char **ref_cache, int *cachePtr) {

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
      return 0;
    }
    //process cell to get reference value
    process_cell(c[row][col], row_dim, col_dim, c, ref_cache, cachePtr);
    memset(buffer, '\0', ptr);
    strcpy(buffer, c[row][col]->output);
    return 0;
  }
  // is digit
  if(regex_match(buffer, "[0-9]+") == 1) {
    return 0;
  }
  //is text
  if(regex_match(buffer, "[a-zA-Z]+") == 1) 
    return 1;
    
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



int process_op(int op, int operand, int answer) {
  switch(op) {
  case 1:
    return (int)(operand + answer);
    break;
  case 2:
    return (int)(answer - operand);
    break;
  case 3:
    return (int)(answer * operand);
    break;
  case 4:
    return (int)(answer / operand);
    break;
  default:
    break;
  }
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
