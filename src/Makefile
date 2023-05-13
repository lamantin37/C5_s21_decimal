CC = gcc
CFLAGS = -std=c11 
SOURCE = $(wildcard s21_*.c)
HEADERS = $(wildcard s21_*.h)
TESTS = $(wildcard test_*)
OBJECTS = $(patsubst %.c,%.o, ${SOURCE})
_UNAME_ := $(shell uname -s)

ifeq ($(_UNAME_), Linux)
	OPEN_CMD = xdg-open
	ADD_LIB := -lm -lcheck -lsubunit
	BREW = .
	BROWSER_APP = firefox 
endif

ifeq ($(_UNAME_), Darwin) # MacOS
	OPEN_CMD = open
	ADD_LIB = -lm -lcheck 
	BREW = ${HOME}/homebrew
	BROWSER_APP = /Applications/Google\ Chrome.app
endif

PATH := $(PATH):$(BREW)/bin
GCOV = --coverage
PROJECT_NAME := s21_decimal

all: clean style ${PROJECT_NAME}.a test gcov_report

$(PROJECT_NAME).a: ${SOURCE} ${HEADERS}
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build a static library 's21_decimal.a'  )
	$(info ----------------------------------------)
	$(CC) -c $(CFLAGS) ${SOURCE}
	ar rc $@ ${OBJECTS}
	ranlib $@	

test: $(TESTS) ${PROJECT_NAME}.a
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build tests                           )
	$(info ----------------------------------------)
	$(CC) $^ $(ADD_LIB) -o $@  
	./$@

debug: $(TESTS) ${PROJECT_NAME}.a
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build tests with debug for valgrind     )
	$(info ----------------------------------------)
	$(CC) -g $^ $(ADD_LIB) -o $@
	./$@

gcov_report: clean lcov ${SOURCE} ${TESTS} 
	$(info                                         )
	$(info ----------------------------------------)
	$(info Create tests coverage report            )
	$(info ----------------------------------------)
	mkdir report
	$(CC) -c $(CFLAGS) $(ADD_LIB) $(GCOV) ${SOURCE} 
	$(CC) -o $@ $(GCOV) $(OBJECTS) $(TESTS) $(ADD_LIB)
	./$@
	lcov -t $@ -o $@.info -c -d . --no-external
	genhtml -o report/ $@.info

ifeq ($(_UNAME_), Darwin) # MacOS
	open $(BROWSER_APP) ./report/index.html
else
	$(BROWSER_APP) ./report/index.html
endif

lcov:
ifeq ("", "$(shell PATH=$(PATH) which lcov)")
	$(error Need to install lcov)
endif

style:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Check for code style and formatting     )
	$(info ----------------------------------------)
	clang-format -style=Google -i *.c *.h 
#	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCE) $(HEADERS) 
#	cp ../../materials/linters/CPPLINT.cfg .
#	python3 ../../materials/linters/cpplint.py --extensions=c $(SOURCE) 
#	python3 ../../materials/linters/cpplint.py --extensions=c $(HEADERS) 

clean:
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/
