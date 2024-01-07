# Compiler and flags
CXX = clang++
CFLAGS:= $(CFLAGS) -std=c++11
SMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = test

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

OBJ_FILES = $(SRC_FILES:.cpp=.o) $(TEST_FILES:.cpp=.o)

TEST_EXECUTABLE = test.out

.PHONY: all clean test game

all: $(TEST_EXECUTABLE)

%.o: %.cpp
	$(CXX) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

$(TEST_EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $(SMLFLAGS) -o $@ $(OBJ_FILES)

debug: $(TEST_EXECUTABLE)
	gdb $(TEST_EXECUTABLE)

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	rm -f $(OBJ_FILES) $(TEST_EXECUTABLE)