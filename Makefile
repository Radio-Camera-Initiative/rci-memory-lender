CC = /usr/bin/g++
FLAGS = -std=c++17
INCLUDE = -I.

LD_FLAGS = -lrt -Wall -lpthread


DEPS = r3.hpp
SOURCES = main.cpp

TARGETS = main

all: $(TARGETS)

$(TARGETS): $(SOURCES) $(DEPS)
	$(CC) $(FLAGS) -o $(TARGETS) -O3 $(INCLUDE) $(SOURCES) $(LD_FLAGS)

# $(TARGETS): r3.o main.o
# 	$(CC) $(FLAGS) $^ -o $@ -O3 $(LDFLAGS)

clean:
	rm -f *.o $(TARGETS)

again: clean $(TARGETS)
