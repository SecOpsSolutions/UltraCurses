VPATH = ./src

objects = bin/Terminal.o bin/IO.o
includes = -Isrc/

CPP = g++ -c -o $@ $< $(includes)

Demo: Demo.cpp $(objects) Terminal.h IO.h Defines.h
	g++ -o $@ $< $(objects) $(includes) -lncurses

run: Demo
	./Demo

bin/Terminal.o: Terminal.cpp Terminal.h IO.h Defines.h
	$(CPP)

bin/IO.o: IO.cpp IO.h Defines.h
	$(CPP)

.PHONY: clean
clean:
	rm -rf bin/* Demo
