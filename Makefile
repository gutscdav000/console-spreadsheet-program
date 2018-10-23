cc = gcc
flags =  -g -ggdb -std=c99

OBJ = main.o load.o cell.o

# executible compilation
spreadsheet: $(OBJ)
	$(cc) -o $@ $^ $(flags) 

cell.o: cell.h cell.c
	$(cc) -c $(flags) cell.c -o cell.o
load.o: load.h load.c
	$(cc) -c $(flags) load.c -o load.o
main.o: main.c
	$(cc) -c $(flags) main.c -o main.o


clean:
	rm -f *.o
	rm -f spreadsheet test

# test suite compilation
test: cell.o test_cell.o load.o
	$(cc) $(flags) -o test test_cell.o cell.o load.o

test_cell.o: test_cell.c 
	$(cc) -c $(flags) -o test_cell.o test_cell.c
