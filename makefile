VPATH = ./src

objects = bin/Terminal.o bin/IO.o
headers = src/uCurses.h src/Terminal.h src/IO.h src/Defines.h
includes = -Isrc/

CPP = g++ -c -o $@ $< $(includes)

.PHONY: all
all: Demo lib/ucurses.a $(headers)
	cp $(headers) inc/

Demo: Demo.cpp lib/ucurses.a Terminal.h IO.h Defines.h
	g++ -o $@ $< $(includes) lib/ucurses.a -lncurses

run: Demo
	./Demo

lib/ucurses.a: $(objects)
	ar rvs lib/ucurses.a $(objects)

bin/Terminal.o: Terminal.cpp Terminal.h IO.h Defines.h
	$(CPP)

bin/IO.o: IO.cpp IO.h Defines.h
	$(CPP)

.PHONY: clean
clean:
	rm -rf bin/* 
	rm -f Demo
	rm -rf lib/*
	rm -rf inc/*
