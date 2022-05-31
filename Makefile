CC = /usr/bin/g++
FLAGS = -std=c++2a -g
INCLUDE = -I. -I/opt/include

LD_FLAGS = -lrt -Wall -lpthread -lidg-cuda -L/opt/lib -lidg-util -lidg-common -lidg -lcasa_ms -lcasa_tables -lcasa_casa


DEPS = lender.hpp
SOURCES = main.cpp

TARGETS = main

all: $(TARGETS)

main: main.cpp $(DEPS)
	$(CC) $(FLAGS) -o main -O3 $(INCLUDE) main.cpp $(LD_FLAGS)

# $(TARGETS): $(SOURCES) $(DEPS)
# 	$(CC) $(FLAGS) -o $(TARGETS) -O3 $(INCLUDE) $(SOURCES) $(LD_FLAGS)

# $(TARGETS): r3.o main.o
# 	$(CC) $(FLAGS) $^ -o $@ -O3 $(LDFLAGS)

clean:
	rm -f *.o $(TARGETS)

again: clean $(TARGETS)
