#include "load.h"  


void load_file(FILE *in, int x, int y, cell_t *c[x][y]) {
  
  
  int row_index = 0, col_index = 0;
  int row = row_index + 1;
  int col = 0x41; // start at A (0x41 : 65)
  
  char temp[150];
  int character;

    
  do {

    int i = 0;
    // read chars into buffer
    while((character = fgetc(in)) != 9 && character != 10 && character != EOF) {
	temp[i++] = ((char)character);
      }
    
    // if character is TAB/NEWLINE read buffer into cell_t
    if((character == 9 || character == 10) && (col_index < y && row_index < x)) {
      
      // if temp has chars read them into input otherwise write #NAN
      if( i > 0)
	sprintf(c[row_index][col_index]->input, "%s", temp);
      else
	sprintf(c[row_index][col_index]->input, "%s", "#NAN");

      //printf("%d,%d   %s\n", row_index, col_index,	\
      //	     c[row_index][col_index]->input);

      // update indeces
      col_index++;
      col++;
    }
    
    // if character is NEWLINE 
    if(character == 10) {
      // fill remaining inputs when newline is called prematurely
      while(col_index < y) {
	sprintf(c[row_index][col_index]->input, "%s", "#NAN");
	col_index++;
      }


      //update indeces
      col_index = 0;
      col = 65 + col_index; // offset for capital letter
      row++;
      row_index++;
    }

    // clear temp buffer
    memset(temp, '\0', i);

  } while(character != EOF);
  printf("\nend of do while\n");

  for(int i = 0; i < x; i++) {
    for(int j = 0; j < y; j++) {
      printf("%d,%d\t%s\n", i, j,c[i][j]->input);
    }
    printf("\n");
  }
  
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

  // save dimensions in array provided to function
  //dimensions [x,y]
  dimensions[0] = num_rows;
  dimensions[1] = max_cols + 1; // 1 more column than tab in file

}

int push_reference(char *ref) {

  int ret = 0;
  //if cache contains ref return true
  if(contains(ref) == 1)
    ret = 1;

  //if cache is full resize
  if(cachePtr >= cache_size) {
    // copy cache
    char **temp = malloc(sizeof(char) * DEFAULT_REF_SIZE * cache_size);
    for(int i = 0; i < cache_size; i++) {
      temp[i] = (char *)malloc(sizeof(char) * DEFAULT_REF_SIZE);
      strcpy(temp[i], reference_cache[i]);
    }
    // resize reference_cache
    free(reference_cache);
    cache_size *= 2;
    reference_cache = (const char **)realloc(temp, sizeof(char) * DEFAULT_REF_SIZE * cache_size);
    free(temp);
  }
  
  // insert ref into array
  reference_cache[cachePtr] = (char*)malloc(sizeof(char) * DEFAULT_REF_SIZE);
  strcpy(((char *)reference_cache[cachePtr]), ref);
  cachePtr++;
  
}


int contains(char *ref) {
  for(int i = 0; i < cachePtr; i++) {
    if(strcmp(reference_cache[i], ref) == 0)
      return 1;
  }
  return 0;
}

void clear_cache() {
  if(reference_cache)
    free(reference_cache);
  
  cache_size = DEFAULT_CACHE_SIZE;
  reference_cache = malloc(sizeof(char) * DEFAULT_REF_SIZE * cache_size); 
}
