CC              = gcc -std=c11
CFLAGS          = `pkg-config --cflags gtk+-3.0` -Wall -Wextra #-Werror
#-fsanitize=address
EXTRA_CFLAGS    = 
#-ffast-math -fdata-sections -ffunction-sections -Wconversion -Wpointer-arith -Wstrict-prototypes -Wundef -pedantic
TEST_CFLAGS     = -I check/include -L check/Cellar/check/0.15.2/lib -lcheck
#-I /usr/local/include -L /usr/local/lib -lcheck -pthread -pthread -lrt -lm
LIB_FLAGS		= -lm `pkg-config --libs gtk+-3.0`
OUTPUT_NAME     = calculator
C_FILES         = stack.c calculator.c gui.c
TEST_C_FILES	= test.c

all: calculator

text_calculator:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(C_FILES) $(LIB_FLAGS) -o $(OUTPUT_NAME)

calculator:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(C_FILES) -o $(OUTPUT_NAME) $(LIB_FLAGS)

debug:
	$(CC) $(CFLAGS) -D name=SDEBUG $(EXTRA_CFLAGS) $(C_FILES) -o $(OUTPUT_NAME) $(LIB_FLAGS)

clean:
	rm -f *.o *.info *.gcno *.gcda gcov_report all

test: calculator.a
	$(CC) -c $(TEST_C_FILES) $(TEST_CFLAGS)
	$(CC) -o $(OUTPUT_NAME) *.o -L . calculator.a $(TEST_CFLAGS)
	$(CC) -o $(OUTPUT_NAME) --coverage $(CFLAGS) $(EXTRA_CFLAGS) *.c $(TEST_CFLAGS) -lcheck
	./$(OUTPUT_NAME)
	rm -f *.o all

calculator.a:
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(C_FILES) -L. -I.
	ar rcs calculator.a *.o
	ar rcs libcalculator.a *.o
	make clean

gcov_report: test
#	$(CC) -o $(OUTPUT_NAME) --coverage $(CFLAGS) $(EXTRA_CFLAGS) *.c $(TEST_CFLAGS)
#	./$(OUTPUT_NAME)
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html
	make clean

rebuild: 
	rm -f $(OUTPUT_NAME)
	make clean
	make all

full_clean:
	rm -f $(OUTPUT_NAME)
	rm -f *.o *.a *.info *.gcno *.gcda all
	rm -rf report
