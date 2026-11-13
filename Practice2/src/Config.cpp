#include "Config.h"

#include <iostream>

void Config::print() const {
    std::cout << "=== Dependency Visualizer ===\n";

    std::cout << "package: " << packageName << '\n';
    std::cout << "repository: " << repository << '\n';
    std::cout << "test-mode: "
              << (testMode ? "true" : "false") << '\n';
    std::cout << "max-depth: " << maxDepth << '\n';
}