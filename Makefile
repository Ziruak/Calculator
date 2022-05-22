CC              = gcc -std=c11
CFLAGS          = -c -Wall -Wextra #-Werror
GUI_CFLAGS		= -c `pkg-config --cflags gtk+-3.0`
#-fsanitize=address
EXTRA_CFLAGS    = 
#-ffast-math -fdata-sections -ffunction-sections -Wconversion -Wpointer-arith -Wstrict-prototypes -Wundef -pedantic
TEST_CFLAGS     = -c -I./check/include -pthread
#-I /usr/local/include -L /usr/local/lib -lcheck -pthread -pthread -lrt -lm

LIB_FLAGS		= -L. -lcalculator -lm
GUI_LIB_FLAGS	= `pkg-config --libs gtk+-3.0`
TEST_LIB_FLAGS	= -L check/Cellar/check/0.15.2/lib -lcheck

OUTPUT_NAME     = calculator
TEST_OUTPUT_NAME= calculator_tests

C_FILES         = stack.c calculator.c
GUI_C_FILES		= f_graph.c gui.c
TEST_C_FILES	= test.c

all: calculator


calculator: libcalculator.a
	$(CC) $(GUI_CFLAGS) $(EXTRA_CFLAGS) $(GUI_C_FILES)
	$(CC) libcalculator.a gui.o f_graph.o -o $(OUTPUT_NAME) $(LIB_FLAGS) $(GUI_LIB_FLAGS)
	make clean

libcalculator.a:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(C_FILES)
	ar rcs libcalculator.a calculator.o stack.o
	make clean

clean:
	rm -f *.o *.info *.gcno *.gcda gcov_report all

test: libcalculator.a
	$(CC) $(TEST_CFLAGS) $(EXTRA_CFLAGS) $(TEST_C_FILES)
	$(CC) libcalculator.a test*.o -o $(TEST_OUTPUT_NAME) $(LIB_FLAGS) $(TEST_LIB_FLAGS)
	./$(TEST_OUTPUT_NAME)
	rm -f *.o all

gcov_report:
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $(EXTRA_CFLAGS) $(C_FILES)
	$(CC) $(TEST_CFLAGS) -fprofile-arcs -ftest-coverage $(EXTRA_CFLAGS) $(TEST_C_FILES)
	$(CC) calculator.o stack.o test*.o -o $(TEST_OUTPUT_NAME) --coverage -L. -lm $(TEST_LIB_FLAGS)
	./$(TEST_OUTPUT_NAME)
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
#	.open /report/index.html
	make clean

full_clean:
	rm -f $(OUTPUT_NAME) $(TEST_OUTPUT_NAME)
	rm -f *.o *.a *.info *.gcno *.gcda all
	rm -rf report
