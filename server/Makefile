CXX = g++
CXXFLAGS =	-g -Wall -pedantic -std=c++0x
LIBS = -pthread

SRC = $(shell find *.cpp lib/*.cpp lib/util/*.cpp)
OBJ = $(SRC:%.cpp=obj/%.o)
BIN = bin/main

all: $(BIN)

obj/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: %.cpp %.hpp lib/defs.hpp lib/MultithreadQueue.hpp GameState.hpp GenObj.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(CXX) $^ -o $@ $(LIBS)
	
clean:
	rm -rf $(OBJ) $(BIN)

run:
	./$(BIN)
