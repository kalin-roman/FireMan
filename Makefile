CC = g++ 
DEBUG = -DDEBUG -g
COMPILER_FLAGS =-Wall -Wfatal-errors
LANG_STD = -std=c++17
INCLUDE_PATH = -I"./libs/"
SRC_FILES = src/*.cpp src/**/*.cpp libs/imgui/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
OBJ_NAME = fire-man

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH)  $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH)  $(SRC_FILES) $(LINKER_FLAGS) $(DEBUG) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)