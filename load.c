#include "load.h"  


void load_file(FILE *in, int x, int y, cell_t *c[x][y]) {
  
  
  int row_index = 0, col_index = 0;
  int row = row_index + 1;
  int col = 0x41; // start at A (0x41 : 65)
  
  char temp[150];
  // first char forces the cell to be read in the do while loop
  int character, first_char = 1;


  // read first char
    character = fgetc(in);
    
  do {
    
    printf("%x ", character);
    
    // if character is TAB read until next \t or \n
    if(character == 9 || first_char == 1) {
      int i = 0;
      // don't skip over first char
      if(first_char == 1 && character != 9)
	temp[i++] = ((char)character);
      
      while((character = fgetc(in)) != 9 && character != 10 && character != EOF) {
	temp[i++] = ((char)character);
      }
      // if temp has chars read them into input otherwise write #NAN
      if( i > 0)
	sprintf(c[row_index][col_index]->input, "%d,%d\t%s", row_index,col_index, temp);
      else
	sprintf(c[row_index][col_index]->input, "%s", "#NAN");
      
      // update indeces
      col_index++;
      col++;
      first_char = 0;
    }
    // if character is NEWLINE
    if(character == 10) {
      //update indeces
      col_index = 0;
      col = 65 + col_index; // offset for capital letter
      row++;
      row_index++;
      first_char = 1;
    }

    character = fgetc(in);
  } while(character != EOF);
  printf("\nend of do while\n");

  /*  for(int i = 0; i <= x; i++) {
    for(int j = 0; j <= y; j++) {
      printf("%s\n", c[i][j]->input);
    }
    printf("\n");
    }*/
      
}


void get_dimensions(FILE *in, int* dimensions) {
  int max_cols = 0, num_rows = 0, iter_cols = 0;
  int character;

  while((character = fgetc(in)) != EOF) {
      // tab
      if(character == 0x9) {
	iter_cols++;
      }
      // new line
      else if(character == 0xa) {
	num_rows++;
	max_cols = iter_cols > max_cols? iter_cols: max_cols;
	iter_cols = 0;
      }
      printf("%d ", character);
    }

      //dimensions [x,y]
  dimensions[0] = num_rows;
  dimensions[1] = max_cols;

}

