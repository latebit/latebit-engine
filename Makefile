# Compiler and flags
CXX = clang++
CFLAGS:= $(CFLAGS) -std=c++11

# Directories
SRC_DIR = src
TEST_DIR = test
EXAMPLES_DIR = examples

# Source and test files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)
EXAMPLES_FILES = $(wildcard $(EXAMPLES_DIR)/*.cpp)

# Object files
OBJ_FILES = $(SRC_FILES:.cpp=.o) $(TEST_FILES:.cpp=.o)

# Executable for the tests
TEST_EXECUTABLE = test/test.out
GAME_EXECUTABLE = examples/game.out

.PHONY: all clean test game

all: $(TEST_EXECUTABLE) $(GAME_EXECUTABLE)

# Compile source files and test files into object files
%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

# Link object files to create the test executable
$(TEST_EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CFLAGS) -o $@ $(OBJ_FILES)

# Link object files to create the game executable
$(GAME_EXECUTABLE): $(SRC_FILES:.cpp=.o) $(EXAMPLES_DIR)/game.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_FILES:.cpp=.o) $(EXAMPLES_DIR)/game.cpp

# Run the examples
game: $(GAME_EXECUTABLE)
	./$(GAME_EXECUTABLE)

# Run the tests
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	rm -f $(OBJ_FILES) $(TEST_EXECUTABLE)