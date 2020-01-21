VPATH = ./src

binaries = bin/
objects = obj/Terminal.o obj/BasicWindow.o
headers = inc/uCurses.h inc/Terminal.h inc/BasicWindow.h inc/Defines.h
includes = -Iinc/
lopts = -Llib/ -lucurses -lncurses

CPP = g++ -std=c++11 -c -o $@ $< $(includes)

.PHONY: all
all: lib/libucurses.a $(headers) bin/demo bin/snake

bin/% : test/%.cpp lib/libucurses.a Terminal.h BasicWindow.h Defines.h
	@-mkdir -p bin
	g++ -std=c++11 -o $@ $< $(includes) $(lopts)

# bin/demo: test/demo.cpp lib/libucurses.a Terminal.h BasicWindow.h Defines.h
# 	g++ -std=c++11 -o $@ $< $(includes) $(lopts)

# bin/snake: test/snake.cpp lib/libucurses.a Terminal.h BasicWindow.h Defines.h
# 	g++ -std=c++11 -o $@ $< $(includes) $(lopts)

inc/%.h: src/%.h
	@-mkdir -p inc
	cp $< $@

lib/libucurses.a: $(objects)
	@-mkdir -p lib/
	ar rvs lib/libucurses.a $(objects)

obj/Terminal.o: Terminal.cpp Terminal.h Defines.h
	@-mkdir -p obj
	$(CPP)

obj/BasicWindow.o: BasicWindow.cpp BasicWindow.h Defines.h Terminal.h
	@-mkdir -p obj
	$(CPP)

.PHONY: clean
clean:
	rm -rf bin/* 
	rm -rf obj/*
	rm -rf lib/*
	rm -rf inc/*
