CXXFLAGS =	-O2 -g -Wall

#This finds all your cc files and places then into SRC. It's equivalent would be
# SRC = src/main.cc src/folder1/func1.cc src/folder1/func2.cc src/folder2/func3.cc

SRC = $(shell find *.cpp lib/*.cpp)

#This tells Make that somewhere below, you are going to convert all your source into 
#objects, so this is like:
# OBJ =  src/main.o src/folder1/func1.o src/folder1/func2.o src/folder2/func3.o

OBJ = $(SRC:%.cpp=%.o)

#Tells make your binary is called artifact_name_here and it should be in bin/
BIN = main

# all is the target (you would run make all from the command line). 'all' is dependent
# on $(BIN)
all: $(BIN)

#$(BIN) is dependent on objects
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) 

#each object file is dependent on its source file, and whenever make needs to create
# an object file, to follow this rule:
%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)
	
clean:
	rm -rf $(OBJ) $(BIN)