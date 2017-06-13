VPATH = ./src

objects = bin/Terminal.o bin/BasicWindow.o
headers = src/uCurses.h src/Terminal.h src/BasicWindow.h src/Defines.h
includes = -Iinc/ -I../MultiThread/inc/
lopts = -Llib/ -lucurses -L../MultiThread/lib/ -lmthreads -lncurses -lpthread

CPP = g++ -c -o $@ $< $(includes)

.PHONY: all
all: Demo lib/libucurses.a $(headers)
	cp $(headers) inc/

Demo: Demo.cpp lib/libucurses.a Terminal.h BasicWindow.h Defines.h
	g++ -o $@ $< $(includes) $(lopts)

run: Demo
	./Demo

lib/libucurses.a: $(objects)
	ar rvs lib/libucurses.a $(objects)

bin/Terminal.o: Terminal.cpp Terminal.h Defines.h
	$(CPP)

bin/BasicWindow.o: BasicWindow.cpp BasicWindow.h Defines.h
	$(CPP)

.PHONY: clean
clean:
	rm -rf bin/* 
	rm -f Demo
	rm -rf lib/*
	rm -rf inc/*
