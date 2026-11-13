
#include "TestRepositoryReader.h"

#include <fstream>
#include <sstream>
#include <cctype>

bool TestRepositoryReader::load(
    const std::string& path,
    std::unordered_map<std::string, std::vector<std::string>>& graph,
    std::string& error
) {
    std::ifstream file(path);

    if (!file.is_open()) {
        error = "Cannot open test repository";
        return false;
    }

    graph.clear();

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::size_t separator = line.find(':');

        if (separator == std::string::npos) {
            error = "Invalid format at line " +
                    std::to_string(lineNumber);
            return false;
        }

        std::string package = line.substr(0, separator);

        std::istringstream packageStream(package);
        packageStream >> package;

        if (package.empty()) {
            error = "Empty package name at line " +
                    std::to_string(lineNumber);
            return false;
        }

        for (char ch : package) {
            if (!std::isupper(static_cast<unsigned char>(ch))) {
                error = "Package names must contain uppercase Latin letters";
                return false;
            }
        }

        if (graph.count(package)) {
            error = "Duplicate package: " + package;
            return false;
        }

        std::vector<std::string> dependencies;

        std::istringstream stream(line.substr(separator + 1));
        std::string dependency;

        while (stream >> dependency) {
            for (char ch : dependency) {
                if (!std::isupper(static_cast<unsigned char>(ch))) {
                    error = "Invalid dependency name at line " +
                            std::to_string(lineNumber);
                    return false;
                }
            }

            dependencies.push_back(dependency);
        }

        graph[package] = dependencies;
    }

    if (file.bad()) {
        error = "Error while reading test repository";
        return false;
    }

    if (graph.empty()) {
        error = "Test repository is empty";
        return false;
    }

    for (const auto& entry : graph) {
        for (const auto& dependency : entry.second) {
            if (!graph.count(dependency)) {
                error = "Unknown dependency '" + dependency +
                        "' referenced by package '" + entry.first + "'";
                return false;
            }
        }
    }

    return true;
}
