CC := g++
CC_FLAGS := -std=c++17 -g -Wall

MAINFILE := src/main.cpp
TESTMAINFILE := tests/test_main.cpp
TARGET := bin/main

SRCS := src/Cell.cpp src/CellStorage.cpp src/Coordinates.cpp src/Wind.cpp src/Fire.cpp

compile_run: clean compile run

run:
	@echo "🚀 Executing..."
	@./$(TARGET)

compile_run_test: compile_test run

compile: echo_nice_icon
	mkdir -p bin
	$(CC) $(CC_FLAGS) $(SRCS) $(MAINFILE) -o $(TARGET)

compile_test: echo_nice_icon
	mkdir -p bin
	$(CC) $(CC_FLAGS) $(SRCS) $(TESTMAINFILE) -o $(TARGET)

echo_nice_icon:
	@echo "🚧 Building..."

clean:
	rm -rf bin/*
