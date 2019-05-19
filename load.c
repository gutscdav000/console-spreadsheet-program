#include "load.h"  


void load_file(FILE *in, int x, int y, cell_t *c[x][y]) {
  
  
  int row_index = 0, col_index = 0;
  
  char temp[160]; // hey this was 150
  int character;

    
  do {

    int i = 0;
    // read chars into buffer
    while((character = fgetc(in)) != 9 && character != 10 && character != EOF && i < 160) {
	temp[i++] = ((char)character);
      }
    temp[i++] = '\0';
    
    // if character is TAB/NEWLINE read buffer into cell_t
    if((character == 9 || character == 10) && (col_index < y && row_index < x)) {
      
      // if temp has chars read them into input otherwise write #NAN
      if( i > 0 && i <= 150)
	sprintf(c[row_index][col_index]->input, "%s", temp);
      else if(i > 150) {
	sprintf(c[row_index][col_index]->input, "%s", "#ERROR");
	sprintf(c[row_index][col_index]->output, "%s", "#ERROR");
	c[row_index][col_index]->hasOutput = 1;
      }
      else
	sprintf(c[row_index][col_index]->input, "%s", "#NAN");
      
      //printf("%d,%d   %s\n", row_index, col_index,	\
      //	     c[row_index][col_index]->input);
      
      // update index
      col_index++;
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
      row_index++;
    }

    // clear temp buffer
    memset(temp, '\0', i);

  } while(character != EOF);
  
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
      
    }

  // save dimensions in array provided to function
  //dimensions [x,y]
  dimensions[0] = num_rows;
  dimensions[1] = max_cols + 1; // 1 more column than tab in file

}

int push_reference(char *ref, char** ref_cache, int *cachePtr) {

  int ret = 0;
  //if cache contains ref return true
  if(contains(ref, ref_cache, cachePtr) == 1)
    ret = 1;

  
  // insert ref into array
  ref_cache[(*cachePtr)] = (char*)malloc(sizeof(char) * DEFAULT_REF_SIZE); 
  strcpy(((char *)ref_cache[(*cachePtr)]), ref);
  (*cachePtr)++;

  return ret;
}


int contains(char *ref, char **ref_cache, int *cachePtr) {
  for(int i = 0; i < *cachePtr; i++) {
    if(strcmp(ref_cache[i], ref) == 0)
      return 1;
  }
  return 0;
}

void clear_cache(char ***ref_cache, int *ptr) {
  if(*ref_cache != NULL)
    free(*ref_cache);

  *ptr = 0;
  *ref_cache = (char**)malloc(sizeof(char) * DEFAULT_REF_SIZE * DEFAULT_CACHE_SIZE); 
}



void print_spreadsheet(char *out, int rows, int cols, cell_t *c[rows][cols]) {
  int fileOut;
  FILE *f;
  
  if(out[0] == '\0') {
    fileOut = 0;
  }
  else {
    // file output
    f = fopen(out, "w");
    fileOut = 1;
  }

  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      printf("%s\t", c[i][j]->output);
      if(fileOut == 1 && f != NULL)
	fprintf(f,"%s\t", c[i][j]->output);
    }
    printf("\n");
    if(fileOut == 1 && f != NULL)
      fprintf(f,"\n");
  }
  
  if(fileOut == 1)
    fclose(f);
}
