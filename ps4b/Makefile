CC = g++
CFLAGS = --std=c++14 -g -Wall -Werror -pedantic
LIB = -lsfml-graphics -lsfml-window -lsfml-system

all: main.cpp lint

Sokoban.o: Sokoban.cpp
	$(CC) $(CFLAGS) Sokoban.cpp -c 

main.cpp: Sokoban.o
	$(CC) $(CFLAGS) Sokoban.o main.cpp -o Sokoban $(LIB)

clean:
	rm -f Sokoban *.o
	
lint:
	cpplint --filter=-runtime/references,-build/c++11 --root=. *
