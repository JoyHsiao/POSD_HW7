all: hw5

hw5: hw5.o
ifeq (${OS}, Windows_NT)
	g++ -g3 -o hw5 hw5.o -lgtest
else
	g++ -g3 -o hw5 hw5.o -lgtest -lpthread
endif

hw5.o: hw5.cpp term.h atom.h number.h variable.h struct.h list.h scanner.h parser.h utParser.h utScanner.h
#utVariable.h utStruct.h utList.h utScanner.h utParser.h
	g++ -std=gnu++0x -c hw5.cpp

clean:
ifeq (${OS}, Windows_NT)
	del *.o hw5
else
	rm -f *.o hw5
endif

stat:
	wc *.h *.cpp
