src = $(wildcard ./src/*.cc)
header = $(wildcard ./include/)
target = main.out
CC = g++
CCFLAGS = -std=c++17 -O2 -Wall -Werror

$(target): $(src)
	$(CC) $(src) -I$(header) -o $(target) $(CCFLAGS)

.PHONY: clean
clean:
	rm -rf $(target)

.PHONY: run
run:
	make && ./$(target)

.PHONY: all
all:
	make && ./$(target) && make clean
