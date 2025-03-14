# CC = gcc
# CFLAGS = -Wall -O3
# END_CFLAGS = -lm

# SRC = spreadsheet.c graph.c hash.c parser.c validate.c
# OBJ = spreadsheet.o graph_node.o hash.c parser.o validate.o

# TARGET = sheet

# # Default target to build the executable
# all : $(TARGET)

# # Rule to create the executable
# $(TARGET) : $(OBJ)
#         $(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(END_CFLAGS)

# # Clean up object files and executable
# clean:
#         rm -f $(OBJ) $(TARGET)

# # Rebuild target: clean and then build the executable
# rebuild: clean all

# report: main.tex
#         pdflatex main.tex

# test: test.c
#         $(CC) $(CFLAGS) -o test test.c $(LDFLAGS)
#         ./test


CC = gcc
CFLAGS = -Wall -O3
END_CFLAGS = -lm

SRC = spreadsheet.c graph.c hash.c parser.c validate.c
OBJ = spreadsheet.o graph.o hash.o parser.o validate.o

DIR = target/release
TARGET = $(DIR)/spreadsheet

# Default target to build the executable
all : $(TARGET)

$(DIR):
	mkdir -p $(DIR)

# Rule to create the executable
$(TARGET): $(OBJ) | $(DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(END_CFLAGS)

# # Rule to compile .c files into .o files
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@
# Individual rules for each object file
spreadsheet.o: spreadsheet.c
	$(CC) $(CFLAGS) -c $< -o $@

graph.o: graph.c
	$(CC) $(CFLAGS) -c $< -o $@

hash.o: hash.c
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c
	$(CC) $(CFLAGS) -c $< -o $@

validate.o: validate.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild target: clean and then build the executable
rebuild: clean all

report: main.tex
	pdflatex main.tex

test: test.c
	$(CC) $(CFLAGS) -o test test.c $(LDFLAGS)
	./test

