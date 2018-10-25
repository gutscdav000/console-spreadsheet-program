cc = gcc
flags =  -g -ggdb -std=c99 #-fno-stack-protector

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
	rm tests/out*

# test suite compilation
test: cell.o test_cell.o load.o
	$(cc) $(flags) -o test test_cell.o cell.o load.o
	./test
	make
	echo "test0.txt spreadsheet test\n"
	./spreadsheet tests/in/test0.txt tests/out0.txt >> tests/trash.txt
	diff tests/sample_out/test0.txt tests/out0.txt
	echo "test1.txt spreadsheet test\n"
	./spreadsheet tests/in/test1.txt tests/out1.txt >> tests/trash.txt
	diff tests/sample_out/test1.txt tests/out1.txt
	echo "test2.txt spreadsheet test\n"
	./spreadsheet tests/in/test2.txt tests/out2.txt >> tests/trash.txt
	diff tests/sample_out/test2.txt tests/out2.txt
	echo "test3.txt spreadsheet test\n"
	./spreadsheet tests/in/test3.txt tests/out3.txt >> tests/trash.txt
	diff tests/sample_out/test3.txt tests/out3.txt

	rm tests/out*
	rm tests/trash.txt


test_cell.o: test_cell.c 
	$(cc) -c $(flags) -o test_cell.o test_cell.c
