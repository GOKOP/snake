COMPILER=g++
FLAGS=-Isrc -lncurses -std=c++17

OBJDIR=obj
SRCDIR=src

DEPS=$(wildcard $(SRCDIR)/*.hpp)
SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p obj
	$(COMPILER) -c -o $@ $< $(FLAGS)

snake: $(OBJ)
	$(COMPILER) $^ -o $@ $(FLAGS)

install:
	cp ./snake /usr/local/bin/coolsnake
