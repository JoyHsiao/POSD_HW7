all: hw3

hw3: hw3.o
ifeq (${OS}, Windows_NT)
	g++ -g3 -o hw3 hw3.o -lgtest
else
	g++ -g3 -o hw3 hw3.o -lgtest -lpthread
endif

hw3.o: hw3.cpp term.h utStruct.h utVariable.h struct.h variable.h atom.h number.h
	g++ -std=gnu++0x -c hw3.cpp

clean:
ifeq (${OS}, Windows_NT)
	del *.o hw3
else
	rm -f *.o hw3
endif

stat:
	wc *.h *.cpp
