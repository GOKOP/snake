COMPILER=g++

compile: 
	$(COMPILER) src/*.cpp -Isrc -lncurses -std=c++0x -o snake;

install:
	cp ./snake /usr/local/bin/coolsnake;
