
CC:=clang

array: src/main.cpp src/array.cpp
	$(CC) src/main.cpp src/array.cpp -std=c++17 -o array

all: array

clean: 
	rm array array.* ||: