
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

TARGET = DependencyVisualizer
SRC_DIR = src
BUILD_DIR = build

PACKAGE ?= nginx
REPO ?= https://dl-cdn.alpinelinux.org/alpine/v3.20/main/x86_64
MAX_DEPTH ?= 3

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Stage 1
stage1: all
	./$(TARGET) --package $(PACKAGE) --repo $(REPO) --test-mode false --max-depth $(MAX_DEPTH)

# Stage 2
stage2: all
	./$(TARGET) --package $(PACKAGE) --repo $(REPO) --test-mode false --max-depth $(MAX_DEPTH)

# Stage 3: test repository
stage3: all
	./$(TARGET) --package A --repo test-repository.txt --test-mode true --max-depth $(MAX_DEPTH)

# Test different graph scenarios
test-depth-0: all
	./$(TARGET) --package A --repo test-repository.txt --test-mode true --max-depth 0

test-depth-1: all
	./$(TARGET) --package A --repo test-repository.txt --test-mode true --max-depth 1

test-depth-2: all
	./$(TARGET) --package A --repo test-repository.txt --test-mode true --max-depth 2

test-cycle: all
	./$(TARGET) --package G --repo test-repository.txt --test-mode true --max-depth 5

test-diamond: all
	./$(TARGET) --package A --repo test-repository.txt --test-mode true --max-depth 5

test-chain: all
	./$(TARGET) --package I --repo test-repository.txt --test-mode true --max-depth 5

# Real Alpine repository
stage3-real: all
	./$(TARGET) --package $(PACKAGE) --repo $(REPO) --test-mode false --max-depth $(MAX_DEPTH)

# Errors
test-errors: all
	./$(TARGET) --package UNKNOWN --repo test-repository.txt --test-mode true --max-depth 3 || true
	./$(TARGET) --package A --repo missing.txt --test-mode true --max-depth 3 || true

help: all
	./$(TARGET) --help

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

.PHONY: all stage1 stage2 stage3 test-depth-0 test-depth-1 test-depth-2 test-cycle test-diamond test-chain stage3-real test-errors help clean rebuild
