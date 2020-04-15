CFLAGS = -g -Wall -pedantic -std=c++14 -Wextra

all: BestFirstSearch

BestFirstSearch: BestFirstSearch.o StSpTree.o
	g++ $(CFLAGS) BestFirstSearch.o StSpTree.o -o BestFirstSearch
BestFirstSearch.o: BestFirstSearch.cpp StSpTree.h
	g++ -c $(CFLAGS) BestFirstSearch.cpp
StSpTree.o: StSpTree.cpp StSpTree.h
	g++ -c $(CFLAGS) StSpTree.cpp
run: all
	./BestFirstSearch input.txt output.txt
clean:
	rm *.o BestFirstSearch
