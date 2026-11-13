
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

TARGET = DependencyVisualizer
SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run application
run: all
	./$(TARGET) --package nginx --repo https://dl-cdn.alpinelinux.org/alpine/v3.20/main/x86_64 --test-mode false --max-depth 3

# Test repository mode
test: all
	./$(TARGET) --package A --repo test-repository.txt --test-mode true --max-depth 2

# Error tests

test-errors: all
	./$(TARGET) --package nginx --repo abc --test-mode false --max-depth 3 || true
	./$(TARGET) --package "" --repo test-repository.txt --test-mode true --max-depth 2 || true
	./$(TARGET) --package nginx --repo test-repository.txt --test-mode yes --max-depth 2 || true
	./$(TARGET) --package nginx --repo test-repository.txt --test-mode true --max-depth -1 || true
	./$(TARGET) --package nginx --repo test-repository.txt --test-mode true --max-depth abc || true
	./$(TARGET) --package nginx --repo missing.txt --test-mode true --max-depth 2 || true
	./$(TARGET) --package nginx --test-mode false --max-depth 2 || true
	./$(TARGET) --package nginx --repo abc --repo xyz --test-mode false --max-depth 2 || true
	./$(TARGET) --unknown test || true


# Help
help: all
	./$(TARGET) --help

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

.PHONY: all run test test-errors help clean rebuild
