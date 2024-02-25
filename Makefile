ifdef WASM
	CXX=emcc
	SDL=-s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
	AR=emar
else
	CXX=clang++
	SDL=-lSDL2 -lSDL2_ttf -lSDL2_mixer
	AR=ar
endif

CFLAGS:=$(CFLAGS) -std=c++20 -fPIC

ifdef DEBUG
	ifdef WASM
		CFLAGS:=-gsource-map
	else
		CFLAGS:=-gdwarf-4
	endif
else
#TODO: Introducing any optimization flag will yield a segmentation fault in the tests
	CFLAGS:=-O0
endif


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

LIB_NAME = latebits

ifdef WASM
	LIB_FILE = $(LIB_DIR)/lib$(LIB_NAME)-wasm.a
else
	LIB_FILE = $(LIB_DIR)/lib$(LIB_NAME).a
endif

TEST_EXECUTABLE = test.out

.PHONY: debug test clean format tidy

all: library

%.o: %.cpp
	$(CXX) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

$(TEST_EXECUTABLE): $(SRC_OBJ_FILES) $(TEST_LIB_OBJ_FILES) $(SUITES_OBJ_FILES) test/main.cpp
	$(CXX) -o $@ test/main.cpp $(CFLAGS) -I$(INC_DIR) $(SDL) $(SRC_OBJ_FILES) $(TEST_LIB_OBJ_FILES)

library: $(SRC_OBJ_FILES)
	$(AR) rcs $(LIB_FILE) $(SRC_OBJ_FILES)

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