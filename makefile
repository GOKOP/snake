COMPILER=g++
FLAGS=-Isrc -lncurses -std=c++0x

OBJDIR=obj
SRCDIR=src

_DEPS=ColorPair.hpp Enums.hpp FruitManager.hpp Menu.hpp Snake.hpp Display.hpp Fruit.hpp IntPair.hpp MenuOption.hpp
_OBJ=ColorPair.o FruitManager.o Menu.o Snake.o Display.o Fruit.o IntPair.o MenuOption.o main.o

DEPS=$(patsubst %,$(SRCDIR)/%,$(_DEPS))
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p obj
	$(COMPILER) -c -o $@ $< $(FLAGS)

snake: $(OBJ)
	$(COMPILER) $^ -o $@ $(FLAGS)

install:
	cp ./snake /usr/local/bin/coolsnake
