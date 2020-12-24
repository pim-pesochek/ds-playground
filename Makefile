
CC:=clang++

array: src/main.cpp src/array.cpp
	$(CC) src/main.cpp src/array.cpp -std=c++17 -o array

list: src/main.cpp src/list.cpp
	$(CC) src/main.cpp src/list.cpp -std=c++17 -o list

tree: src/main.cpp src/tree.cpp
	$(CC) src/main.cpp src/tree.cpp -std=c++17 -o tree

all: array list

clean: 
	rm array array.* list list.* ||: