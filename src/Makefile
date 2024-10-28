GCC = gcc
CFLAGS = -c -std=c11 -Wall -Werror -Wextra
LFLAGS = -lm -lcheck # -g -fsanitize=address
SOURCE = */s21_*.c
STATIC_LIB = s21_decimal.a
CHECK = checkmk test/*/*.check > test.c && checkmk clean_mode=1  test/*/*.check > test.c 
BINARY = s21_test
OBJECTS = $(patsubst %.c,%.o, *.o)
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LFLAGS = -lcheck -lm -lsubunit #-g -fsanitize=address
endif

all: $(STATIC_LIB)

$(STATIC_LIB):
	$(GCC) $(CFLAGS) $(SOURCE) 
	ar rc $@ $(OBJECTS)
	ranlib $@
	cp $@ lib$@
	rm -rf *.o

test: $(STATIC_LIB)
	$(CHECK)
	$(GCC) -o $(BINARY) test.c $(STATIC_LIB) $(LFLAGS)
	./$(BINARY)

clang: 
	cp ../materials/linters/.clang-format .clang-format 
	clang-format -i */*.c */*.h */*/*.check
	rm -rf .clang-format

gcov_report: 
	$(CHECK)
	$(GCC) --coverage $(SOURCE) test.c -o $(BINARY) $(LFLAGS)
	./$(BINARY)
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info $(BINARY) test.c
clean:
	rm -rf *.o *.a $(STATIC_LIB) $(BINARY) report test.c *.dSYM *.gcno *.gcda
