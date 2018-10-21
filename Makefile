cc = gcc
flags =  -g -ggdb -std=c99

OBJ = main.o load.o cell.o


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
	rm -f spreadsheet