COMPILER=g++

compile: 
	$(COMPILER) src/*.cpp -Isrc -lncursesw -std=c++0x -o snake;

install:
	cp ./snake /usr/local/bin/coolsnake;
