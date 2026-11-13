
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

# Stage 2 with custom package
stage2-custom: all
	./$(TARGET) --package $(PACKAGE) --repo $(REPO) --test-mode false --max-depth $(MAX_DEPTH)

stage2-errors: all
	./$(TARGET) --package nonexistent-package-123 --repo $(REPO) --test-mode false --max-depth 3 || true
	./$(TARGET) --package nginx --repo https://invalid.example.invalid/repo --test-mode false --max-depth 3 || true

# Error tests
test-errors: all
	./$(TARGET) --package nginx --repo abc --test-mode false --max-depth 3 || true
	./$(TARGET) --package "" --repo test-repository.txt --test-mode true --max-depth 2 || true
	./$(TARGET) --package nginx --repo test-repository.txt --test-mode yes --max-depth 2 || true
	./$(TARGET) --package nginx --repo test-repository.txt --test-mode true --max-depth -1 || true
	./$(TARGET) --package nginx --repo test-repository.txt --test-mode true --max-depth abc || true
	./$(TARGET) --package nginx --repo missing.txt --test-mode true --max-depth 2 || true
	./$(TARGET) --unknown test || true

help: all
	./$(TARGET) --help

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

.PHONY: all stage1 stage2 stage2-custom test-errors help clean rebuild
