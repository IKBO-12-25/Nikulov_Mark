#include "ArgumentParser.h"
#include "RepositoryValidator.h"

#include <charconv>
#include <string>
#include <iostream>
#include <unordered_map>
#include <charconv>
#include <string>

void ArgumentParser::printHelp() {
    std::cout
        << "Dependency Visualizer\n\n"
        << "Usage:\n"
        << "  DependencyVisualizer [options]\n\n"
        << "Options:\n"
        << "  --package NAME       Package name\n"
        << "  --repo URL_OR_PATH   Repository URL or test file path\n"
        << "  --test-mode VALUE    true or false\n"
        << "  --max-depth NUMBER  Maximum dependency depth\n"
        << "  --help               Show this help message\n";
}

bool ArgumentParser::parse(
    int argc,
    char* argv[],
    Config& config
) {
    std::unordered_map<std::string, std::string> arguments;

    for (int i = 1; i < argc; ++i) {
        std::string key = argv[i];

        if (key == "--help") {
            if (argc != 2) {
                std::cerr << "Error: --help cannot be combined with other arguments\n";
                return false;
            }

            printHelp();
            return false;
        }

        if (key != "--package" &&
            key != "--repo" &&
            key != "--test-mode" &&
            key != "--max-depth") {
            std::cerr << "Error: Unknown parameter: "
                      << key << '\n';
            return false;
        }

        if (arguments.count(key)) {
            std::cerr << "Error: Duplicate parameter: "
                      << key << '\n';
            return false;
        }

        if (i + 1 >= argc ||
            std::string(argv[i + 1]).rfind("--", 0) == 0) {
            std::cerr << "Error: Missing value for "
                      << key << '\n';
            return false;
        }

        arguments[key] = argv[++i];
    }

    const std::string required[] = {
        "--package",
        "--repo",
        "--test-mode",
        "--max-depth"
    };

    for (const auto& key : required) {
        if (!arguments.count(key)) {
            std::cerr << "Error: Missing required parameter: "
                      << key << '\n';
            return false;
        }
    }

    config.packageName = arguments["--package"];
    config.repository = arguments["--repo"];

    if (config.packageName.empty()) {
        std::cerr << "Error: Package name cannot be empty\n";
        return false;
    }

    for (char ch : config.packageName) {
        if (!((ch >= 'a' && ch <= 'z') ||
              (ch >= 'A' && ch <= 'Z') ||
              (ch >= '0' && ch <= '9') ||
              ch == '+' || ch == '_' ||
              ch == '.' || ch == '-')) {
            std::cerr << "Error: Invalid package name\n";
            return false;
        }
    }

    const std::string& mode = arguments["--test-mode"];

    if (mode == "true") {
        config.testMode = true;
    }
    else if (mode == "false") {
        config.testMode = false;
    }
    else {
        std::cerr << "Error: --test-mode must be true or false\n";
        return false;
    }

    const std::string& depth = arguments["--max-depth"];

    if (depth.empty()) {
        std::cerr << "Error: Maximum depth cannot be empty\n";
        return false;
    }

    auto result = std::from_chars(
        depth.data(),
        depth.data() + depth.size(),
        config.maxDepth
    );

    if (result.ec != std::errc() ||
        result.ptr != depth.data() + depth.size() ||
        config.maxDepth < 0) {
        std::cerr
            << "Error: --max-depth must be a non-negative integer\n";
        return false;
    }

    if (config.repository.empty()) {
        std::cerr << "Error: Repository cannot be empty\n";
        return false;
    }

    if (config.testMode) {
        if (!RepositoryValidator::isValidTestFile(config.repository)) {
            std::cerr
                << "Error: Test repository must be an existing regular file\n";
            return false;
        }
    }
    else {
        if (!RepositoryValidator::isValidUrl(config.repository)) {
            std::cerr
                << "Error: Repository must be a valid HTTP or HTTPS URL\n";
            return false;
        }
    }

    return true;
}
