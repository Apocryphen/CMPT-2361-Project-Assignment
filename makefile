CC := clang++
CFLAGS := -Wall -Wextra -Wpedantic
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
	rm $(BIN)
	rm -rf *.dSYM/
