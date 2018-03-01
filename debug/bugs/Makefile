.PHONY: default help object executable all clean segfault dynamic static nobugs
CC = gcc 
CFLAGS = -g -Wall

LD = $(CC)

SOURCE_C = $(wildcard *.c)
OBJECTS_C = $(patsubst %.c, %_c.o, $(SOURCE_C))
EXECUTABLES = $(patsubst %.c, %.e, $(SOURCE_C))

default: all

help:
	@echo "\
Options:\n\n\
  make all:           make every executable (default)\n\
  make segfault:      example with a segfault\n\
  make dynamic:       dynamic arrays with no segfault (doesn't mean no errors!)\n\
  make static:        static arrays, wrong result\n\
  make nobugs:        properly working\n\
  make clean:         delete output files\n\
  make help:          display this help"

objects: $(OBJECTS_C)

executable: $(EXECUTABLES)

all: objects executable

segfault: add_array_segfault.e 

dynamic: add_array_dynamic.e 

static: add_array_static.e 

nobugs: add_array_nobugs.e 

%_c.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.e: %_c.o
	$(LD) $^ -o $@

clean:
	rm -rfv $(OBJECTS_C) $(EXECUTABLES)
