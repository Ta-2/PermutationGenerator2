dev: dev.o
	g++ -Wall -o dev dev.o

dev.o: PermutationGenerator.cpp
	g++ -Wall -o dev.o -c PermutationGenerator.cpp

.PHONY: clean

clean:
	-rm dev dev.o
