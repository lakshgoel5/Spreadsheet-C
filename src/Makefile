CC = gcc
CFLAGS = -Wall -O3
END_CFLAGS = -lm

SRC = spreadsheet.c graph.c hash.c parser.c validate.c vector.c
OBJ = spreadsheet.o graph.o hash.o parser.o validate.o vector.o

DIR = target/release
TARGET = $(DIR)/spreadsheet

PDF_FILE = report.pdf

# Default target to build the executable
all : $(TARGET)

$(DIR):
	mkdir -p $(DIR)

# Rule to create the executable
$(TARGET): $(OBJ) $(DIR) clean
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(END_CFLAGS)

# # Rule to compile .c files into .o files
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@
# Individual rules for each object file
spreadsheet.o: spreadsheet.c
	@$(CC) $(CFLAGS) -c $< -o $@

graph.o: graph.c
	@$(CC) $(CFLAGS) -c $< -o $@

hash.o: hash.c
	@$(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c
	@$(CC) $(CFLAGS) -c $< -o $@

validate.o: validate.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	@rm -f $(OBJ)

# Rebuild target: clean and then build the executable
rebuild: clean all

report: $(PDF_FILE)

$(PDF_FILE): report.tex
	pdflatex report.tex

test: test.c graph.c hash.c parser.c validate.c vector.c
	@$(TARGET) test tests/test_12_15.txt > tests/out_12_15.txt
	@diff -q tests/out_12_15.txt tests/expected_out_12_15.txt
	@echo "Test case 1 passed"
	@$(TARGET) test tests/test1.txt > tests/out_test1.txt
	@diff -q tests/out_test1.txt tests/expected_out_test1.txt
	@echo "Test case 2 passed"
	@$(TARGET) test tests/test2.txt > tests/out_test2.txt
	@diff -q tests/out_test2.txt tests/expected_out_test2.txt
	@echo "Test case 3 passed"
	@$(TARGET) test tests/test3.txt > tests/out_test3.txt
	@diff -q tests/out_test3.txt tests/expected_out_test3.txt
	@echo "Test case 4 passed"
	@$(TARGET) test tests/test_500_1000.txt > tests/out_test_500_1000.txt
	@echo "Test case 5 passed"
	@$(TARGET) test tests/test4.txt; \
	if [ $$? -ne 1 ]; then \
		echo "Test 6 failed: Expected exit code 1, but got $$?"; \
		exit 1; \
	else \
		echo "Test 6 passed: Program correctly returned exit code 1"; \
	fi

	cc -o test test.c graph.c hash.c parser.c validate.c vector.c -lcheck -lm -lsubunit -pthread
	./test
