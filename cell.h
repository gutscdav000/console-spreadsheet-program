#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

// cell structure
typedef struct cell {
  char input[150];
  char output[100];
  int hasOutput;
} cell_t;

// Macros for converting characters to table indeces and visa versa
// col_index -> char & char -> col_index macros
// int -> char & char -> int [65:'A', 90:'Z']
#define col_to_char(i) ((char)(i + 65))
#define char_to_col(c) (((int)(c)) - 65)

// row_index -> char & char -> row_index macros
// int -> char & char -> int [48:'0', 57: '0']
#define row_to_char(i) ((char)(i + 48))
#define char_to_row(c) (((int)c) - 48)



/*purpose: this function process an individual cell taking it's input and generating output both of which are stored in the cell struct
 *
 *@param: cell_t *cell - the individual cell bein processed
 *@param: int row_dim - the number of rows of the 2d cell pointer array
 *@param: int col_dim - the number of columns of the 2d cell pointer array
 *@param: cell_t *c[row_dim][col_dim] - pointer to the 2d array of cells containing the spreadsheet data
 *@param: char **ref_cache - pointer to the array storing the references for the current formula. used to check for cyclicle references
 *@param: int *cachePtr - pointer to the variable maintaining the size of **ref_cache. used to check for cyclicle references
 * NOTE: this function sets cell->output and cell->hasOutput = 1. calls process_formula()
 */
char* process_cell(cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr);

/*purpose: this function processes formulas that are g
 *
 *@param char *formula - pointer to the cstring containing the formula to be processed. (also serves as the return register!)
 *@param: int row_dim - the number of rows of the 2d cell pointer array
 *@param: int col_dim - the number of columns of the 2d cell pointer array
 *@param: cell_t *c[row_dim][col_dim] - pointer to the 2d array of cells containing the spreadsheet data
*@param: char **ref_cache - pointer to the array storing the references for the current formula used to check for cyclicle references.
 *@param: int *cachePtr - pointer to the variable maintaining the size of **ref_cache. used to check for cyclicle references
 */
void process_formula(char *formula, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr);

/*purpose: this function takes input and returns a value > 0 IFF  it's an operation otherwise it returns 0
 *
 *@param: char c - the character to be processed
 *@return: an integer value corresponding with an operation or 0
 */
int op_check(char c);

/*purpose: this function takes an operand from a cell's input and checks for cyclicle references, empty cells, and passes them back via buffer. it also returns 1(true) IFF the cell is text
 *
 *@param: char *buffer - pointer to the cstring containing the operand. also serves as return register if #ERROR or #NAN conditions are met.
 *@param: int row_dim - the number of rows of the 2d cell pointer array
 *@param: int col_dim - the number of columns of the 2d cell pointer array
 *@param: cell_t *c[row_dim][col_dim] - pointer to the 2d array of cells containing the spreadsheet data
*@param: char **ref_cache - pointer to the array storing the references for the current formula used to check for cyclicle references.
 *@param: int *cachePtr - pointer to the variable maintaining the size of **ref_cache. used to check for cyclicle references
 *@return: returns 1 if buffer is text otherwise returns 0
 */
int process_operand(char* buffer, int row_dim, int col_dim, cell_t *c[row_dim][col_dim], char** ref_cache, int *cachePtr);


/*purpose: this function compiles and executes posix regular expressions
 *
 *@param: char *str - the cstring to be searched for a pattern
 *@param: char *pattern - the regex pattern to be matched
 *@return: returns 1 if there is a match otherwise 0
 */
int regex_match( char *str, char *pattern);

/* purpose: this function takes aan operation code int from [1,4] and two operands and returns the computation
 *
 *@param: op an opcode that designates the operation to be performed:
 * 1 = '+', 2 = '-', 3 = '*', 4 = '/'
 *@param: operand - corresponds with the second operand
 *@param: answer - corresponds with the first operand
 *@return: the integer value of the computation
 */
int process_op(int op, int operand, int answer);

/*purpose: this function returns the cell reference (e.g. A1, B2) of a cell given a pointer to the cell in the spreadsheet 2d array of cells
 *
 *@param: char *ref - this is the return register the reference will be written into
 *@param: int row_dim - the number of rows of the 2d cell pointer array
 *@param: int col_dim - the number of columns of the 2d cell pointer array
 *@param: cell_t *c[row_dim][col_dim] - pointer to the 2d array of cells containing the spreadsheet data
 *@param: char **ref_cache - pointer to the array storing the references for the current formula used to check for cyclicle references.
 *@param: int *cachePtr - pointer to the variable maintaining the size of **ref_cache. used to check for cyclicle references
 */
void get_cell_posn(char *ref, cell_t *cell, int row_dim, int col_dim, cell_t *c[row_dim][col_dim]);
