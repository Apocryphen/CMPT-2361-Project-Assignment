CC := clang++
CFLAGS := -Wall -Wextra -Wpedantic -std=c++17
BIN := main
SRC := $(wildcard *.cpp)

default: 
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

test:
	echo $(SRC)

debug: CFLAGS += -g
debug: default 

.PHONY: clean
clean:
	-rm $(BIN)
	-rm -rf *.dSYM/
	-rm .DS_Store
