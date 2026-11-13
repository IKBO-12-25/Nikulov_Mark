
#include "ApkIndexParser.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <utility>

namespace {

std::string getPackageName(const std::string& dependency) {
    std::size_t position = dependency.find_first_of("=<>~");
    return dependency.substr(0, position);
}

void addUnique(
    std::vector<std::string>& destination,
    const std::string& value
) {
    if (std::find(destination.begin(), destination.end(), value)
        == destination.end()) {
        destination.push_back(value);
    }
}

}

bool ApkIndexParser::loadGraph(
    const std::string& indexPath,
    Graph& graph,
    std::string& error
) {
    std::ifstream file(indexPath);

    if (!file.is_open()) {
        error = "Cannot open APKINDEX file";
        return false;
    }

    graph.clear();

    std::unordered_map<std::string, std::string> providers;

    std::string line;
    std::string currentPackage;
    std::vector<std::string> currentDependencies;
    std::vector<std::string> currentProvides;

    auto finishRecord = [&]() {
        if (!currentPackage.empty()) {
            auto& dependencies = graph[currentPackage];

            for (const auto& dependency : currentDependencies) {
                addUnique(dependencies, dependency);
            }

            for (const auto& provider : currentProvides) {
                providers[provider] = currentPackage;
            }
        }

        currentPackage.clear();
        currentDependencies.clear();
        currentProvides.clear();
    };

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            finishRecord();
            continue;
        }

        if (line.rfind("P:", 0) == 0) {
            currentPackage = line.substr(2);
        }
        else if (line.rfind("D:", 0) == 0) {
            std::istringstream stream(line.substr(2));
            std::string value;

            while (stream >> value) {
                currentDependencies.push_back(value);
            }
        }
        else if (line.rfind("p:", 0) == 0) {
            std::istringstream stream(line.substr(2));
            std::string value;

            while (stream >> value) {
                std::string capability = getPackageName(value);
                providers.emplace(capability, currentPackage);            
            }
        }
    }

    finishRecord();

    if (file.bad()) {
        error = "Error while reading APKINDEX";
        return false;
    }

    if (graph.empty()) {
        error = "Invalid APKINDEX: no package records found";
        return false;
    }

    for (auto& entry : graph) {
        std::vector<std::string> resolved;

        for (const auto& dependency : entry.second) {
            std::string name = getPackageName(dependency);

            if (graph.count(name)) {
                addUnique(resolved, name);
                continue;
            }

            auto provider = providers.find(name);

            if (provider != providers.end()) {
                addUnique(resolved, provider->second);
            }
        }

        entry.second = std::move(resolved);
    }

    return true;
}

bool ApkIndexParser::getDependencies(
    const std::string& indexPath,
    const std::string& packageName,
    std::vector<std::string>& dependencies,
    std::string& error
) {
    Graph graph;

    if (!loadGraph(indexPath, graph, error)) {
        return false;
    }

    auto it = graph.find(packageName);

    if (it == graph.end()) {
        error = "Package not found: " + packageName;
        return false;
    }

    dependencies = it->second;
    return true;
}
