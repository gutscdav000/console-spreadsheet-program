# console-spreadsheet-program

This is a spreadsheet application for the command line interface. It will process files with integers, formulas, as well as text. Some of it's features include string concatenation using '+' operator and processing formula's where parenthesis parsing is required: (eg. =(5+A1)/14).

## compiling and running the program

```
make
./spreadsheet <input file>
./spreadsheet <input file> <output file>
```

## auxillary make functions
```
make clean
make test
```
make test runs the test spreadsheets i made in the test forlder. The input files  are found in tests/in and the output files are found in tests/sample_out. make test compares the expected output from sample_out to the program's actual output using the diff unix cli program.

### NOTE: all other documentation may be found in the header files of the project.
