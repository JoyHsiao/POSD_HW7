all: hw6

hw6: hw6.o
ifeq (${OS}, Windows_NT)
	g++ -g3 -o hw6 hw6.o -lgtest
else
	g++ -g3 -o hw6 hw6.o -lgtest -lpthread
endif

hw6.o: hw6.cpp term.h atom.h number.h variable.h struct.h list.h scanner.h parser.h utParser.h utScanner.h node.h
#utVariable.h utStruct.h utList.h utScanner.h utParser.h
	g++ -std=gnu++0x -c hw6.cpp

clean:
ifeq (${OS}, Windows_NT)
	del *.o hw6
else
	rm -f *.o hw6
endif

stat:
	wc *.h *.cpp
