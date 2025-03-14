# Overview
The project consists of the following C files:
- spreadsheet.c
- graph.c
- hash.c
- parser.c
- validate.c
- vector.c

and the following header files:
- spreadsheet.h
- graph.h
- hash.h
- parser.h
- validate.h
- vector.h
- coordinate.h

`spreadsheet.c` contains the main function that gets executed.

Data is stored in `Nodes` inside a matrix, named `graph`. Values are stored in a `grid` of `int` instances, for easier access to values.

## General workflow
`main` in `spreadsheet.c` reads a string as input (command).
It gives in the command to `parser.c` to recognise the instruction.
`parser.c` checks the validity of the command using `validate.c`, and if valid, breaks it down into meaningful substrings. It then extracts the operation performed and the cells and values involved, and returns them to `spreadsheet.c`.
Based on the returned values from `parser.c`, various functions are called and used, defined inside `spreadsheet.c`, `vector.h`, `graph.c` and `hash.c`. Finally, `spreadsheet.c` displays the grid (if display is enabled) and prints the time taken and command status.
This continues in a loop, expecting new commands at each iteration.

For the first command (forming the spreadsheet), the function `process_first` is called in `spreadsheet.c` itself. This creates `graph` and `grid`, which form the basic data elements throughout the project.

# Instructions to run
Run the following commands in the given order.
1. `git clone <URL>`
2. `cd "repository-name"`
3. `make`
4. `cd target/release`
5. `./spreadsheet _ _`

## Syntax rules
- The commands entered in the program must not contain whitespaces, unless for `scroll_to <CELL>`
- The single-letter commands (`w`, `a`, `s`, `d`, `q`) must be lower case.
