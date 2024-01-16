CC := g++
CC_FLAGS := -std=c++17 -g

MAINFILE := src/main.cpp
TESTMAINFILE := tests/test_main.cpp
TARGET := bin/main

SRCS := src/Cell.cpp src/CellStorage.cpp

run: compile
	@echo "🚀 Executing..."
	@./$(TARGET)

compile: echo_nice_icon
	$(CC) $(CC_FLAGS) $(SRCS) $(MAINFILE) -o $(TARGET)

compile_test: echo_nice_icon
	$(CC) $(CC_FLAGS) $(SRCS) $(TESTMAINFILE) -o $(TARGET)

echo_nice_icon:
	@echo "🚧 Building..."