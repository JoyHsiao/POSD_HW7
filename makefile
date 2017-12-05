all: hw7

hw7: hw7.o global.o node.o parser.o struct.o list.o term.o
ifeq (${OS}, Windows_NT)
	g++ -g3 -o hw7 hw7.o global.o node.o parser.o struct.o term.o list.o-lgtest
else
	g++ -g3 -o hw7 hw7.o global.o node.o parser.o struct.o term.o list.o -lgtest -lpthread
endif

#utVariable.h utStruct.h utList.h utScanner.h utParser.h
hw7.o: hw7.cpp term.h atom.h number.h variable.h list.h scanner.h iterator.h utIterator.h
	g++ -std=gnu++0x -c hw7.cpp

global.o: global.cpp global.h
	g++ -std=gnu++0x -c global.cpp
	
node.o: node.cpp node.h
	g++ -std=gnu++0x -c node.cpp

parser.o: parser.cpp parser.h
	g++ -std=gnu++0x -c parser.cpp

term.o: term.cpp term.h
	g++ -std=gnu++0x -c term.cpp

list.o: list.cpp list.h
	g++ -std=gnu++0x -c list.cpp

struct.o: struct.cpp struct.h
	g++ -std=gnu++0x -c struct.cpp

clean:
ifeq (${OS}, Windows_NT)
	del *.o hw7
else
	rm -f *.o hw7
endif

stat:
	wc *.h *.cpp
