
CC:=clang

array: src/main.cpp src/array.cpp
	$(CC) src/main.cpp src/array.cpp -g -o array

all: array

clean: 
	rm array array.* ||: