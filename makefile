all: hw7

hw7: hw7.o global.o node.o parser.o
ifeq (${OS}, Windows_NT)
	g++ -g3 -o hw7 hw7.o global.o node.o parser.o -lgtest
else
	g++ -g3 -o hw7 hw7.o global.o node.o parser.o -lgtest -lpthread
endif

hw7.o: hw7.cpp global.h term.h atom.h number.h variable.h struct.h list.h scanner.h parser.h utParser.h utScanner.h node.h
#utVariable.h utStruct.h utList.h utScanner.h utParser.h
	g++ -std=gnu++0x -c hw7.cpp

global.o: global.cpp global.h
	g++ -std=gnu++0x -c global.cpp
	
node.o: node.cpp node.h
	g++ -std=gnu++0x -c node.cpp

parser.o: parser.cpp parser.h
	g++ -std=gnu++0x -c parser.cpp

clean:
ifeq (${OS}, Windows_NT)
	del *.o hw7
else
	rm -f *.o hw7
endif

stat:
	wc *.h *.cpp
