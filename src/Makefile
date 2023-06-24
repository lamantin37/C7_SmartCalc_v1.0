CC = gcc
CFLAGS = -std=c11 `pkg-config --cflags gtk+-3.0`
SOURCE = $(wildcard s21_smartcalc.c s21_stack.c)
HEADERS = $(wildcard s21_smartcalc.h s21_stack.h)
TESTS = test_21_smartcalc.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCE))
_UNAME_ := $(shell uname -s)

ifeq ($(_UNAME_), Linux)
	OPEN_CMD = xdg-open
	ADD_LIB := `pkg-config --libs gtk+-3.0` -lm -lcheck -lsubunit
	BREW = .
	BROWSER_APP = firefox
endif

ifeq ($(_UNAME_), Darwin) # MacOS
	OPEN_CMD = open
	ADD_LIB = `pkg-config --libs gtk+-3.0` -lm -lcheck
	BREW = ${HOME}/homebrew
	BROWSER_APP = /Applications/Google\ Chrome.app
endif

PATH := $(PATH):$(BREW)/bin
GCOV = --coverage
PROJECT_NAME := s21_smartcalc

all: clean style $(PROJECT_NAME).a test gcov_report

$(PROJECT_NAME).a: $(SOURCE) $(HEADERS)
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build a static library 's21_smartcalc.a')
	$(info ----------------------------------------)
	$(CC) -c $(CFLAGS) $(SOURCE)
	ar rc $@ $(OBJECTS)
	ranlib $@

test: $(TESTS) $(PROJECT_NAME).a
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build tests                           )
	$(info ----------------------------------------)
	$(CC) $^ $(ADD_LIB) -o $@
	./$@

gcov_report: clean lcov $(TESTS)
	$(info                                         )
	$(info ----------------------------------------)
	$(info Create tests coverage report            )
	$(info ----------------------------------------)
	mkdir report
	$(CC) -c $(CFLAGS) $(ADD_LIB) $(GCOV) $(SOURCE)
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

clean:
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.gch
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/

install:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Install the project                     )
	$(info ----------------------------------------)
	mkdir -p /path/to/installation
	cp $(PROJECT_NAME).a /path/to/installation/$(PROJECT_NAME).a
	cp $(HEADERS) /path/to/installation/
	# Add any additional installation steps

uninstall:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Uninstall the project                   )
	$(info ----------------------------------------)
	rm -rf /path/to/installation/$(PROJECT_NAME).a
	rm -rf /path/to/installation/$(HEADERS)
	# Add any additional uninstallation steps

dvi:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Generate DVI file                        )
	$(info ----------------------------------------)
	latex my_document.tex
	# Add any additional DVI generation steps

dist:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Create a distribution package           )
	$(info ----------------------------------------)
	mkdir -p dist/$(PROJECT_NAME)
	cp $(PROJECT_NAME).a dist/$(PROJECT_NAME)/$(PROJECT_NAME).a
	cp $(HEADERS) dist/$(PROJECT_NAME)/
	cp README.md dist/$(PROJECT_NAME)/
	# Add any additional distribution packaging steps

.PHONY: all install uninstall clean dvi dist
