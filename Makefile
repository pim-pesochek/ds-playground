
CC:=clang

array: src/main.cpp src/array.cpp
	$(CC) src/main.cpp src/array.cpp -std=c++17 -o array

list: src/main.cpp src/list.cpp
	$(CC) src/main.cpp src/list.cpp -std=c++17 -o list

all: array list

clean: 
	rm array array.* list list.* ||: