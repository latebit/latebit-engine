CXX = clang++
CFLAGS:= $(CFLAGS) -std=c++20 -fPIC -gdwarf-4
SMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src
INC_DIR = include
TEST_DIR = test
LIB_DIR = lib

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_LIB_FILES = $(wildcard $(TEST_DIR)/lib/*.cpp)
SUITES_FILES = $(wildcard $(TEST_DIR)/suites/*.cpp)

SRC_OBJ_FILES = $(SRC_FILES:.cpp=.o)
TEST_LIB_OBJ_FILES = $(TEST_LIB_FILES:.cpp=.o)
SUITES_OBJ_FILES = $(SUITES_FILES:.cpp=.o)

LIB_NAME = dragonfly
LIB_FILE = $(LIB_DIR)/lib$(LIB_NAME).a

TEST_EXECUTABLE = test.out

.PHONY: debug test clean format tidy

all: $(TEST_EXECUTABLE) $(LIB_FILE)

%.o: %.cpp
	$(CXX) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

$(TEST_EXECUTABLE): $(SRC_OBJ_FILES) $(TEST_LIB_OBJ_FILES) $(SUITES_OBJ_FILES) test/main.cpp
	$(CXX) -o $@ test/main.cpp $(CFLAGS) -I$(INC_DIR) $(SMLFLAGS) $(SRC_OBJ_FILES) $(TEST_LIB_OBJ_FILES)

$(LIB_FILE): $(SRC_OBJ_FILES)
	ar rcs $@ $(SRC_OBJ_FILES)

debug: $(TEST_EXECUTABLE)
	gdb $(TEST_EXECUTABLE)
	rm $(TEST_EXECUTABLE)

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)
	rm $(TEST_EXECUTABLE)

memory: $(TEST_EXECUTABLE)
	valgrind --track-origins=yes --leak-check=full ./$(TEST_EXECUTABLE)
	rm $(TEST_EXECUTABLE)

clean:
	rm -f $(SRC_OBJ_FILES) $(SUITES_OBJ_FILES) $(TEST_LIB_OBJ_FILES) $(TEST_EXECUTABLE) $(LIB_FILE)

format:
	clang-format -i $(SRC_FILES) $(TEST_LIB_FILES) $(SUITES_FILES) $(INC_DIR)/*.h

tidy:
	clang-tidy --fix --fix-errors --fix-notes $(SRC_FILES) $(TEST_LIB_FILES) $(SUITES_FILES) -- $(CFLAGS) -I$(INC_DIR)