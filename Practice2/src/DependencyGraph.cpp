#include "DependencyGraph.h"
#include <iostream>

DependencyGraph::DependencyGraph(const Graph& graph)
    : graph(graph) {}

bool DependencyGraph::contains(const std::string& package) const {
    return graph.count(package) > 0;
}

void DependencyGraph::dfs(
    const std::string& package,
    int depth,
    int maxDepth,
    std::unordered_set<std::string>& visited,
    std::unordered_set<std::string>& recursionStack
) const {
    if (depth >= maxDepth) {
        return;
    }

    auto it = graph.find(package);

    if (it == graph.end()) {
        return;
    }

    for (const auto& dependency : it->second) {
        std::cout << std::string((depth + 1) * 4, ' ')
                  << "|-- " << dependency;

        if (recursionStack.count(dependency)) {
            std::cout << " [cycle]\n";
            continue;
        }

        if (visited.count(dependency)) {
            std::cout << " [already visited]\n";
            continue;
        }

        std::cout << '\n';

        visited.insert(dependency);
        recursionStack.insert(dependency);

        dfs(dependency, depth + 1, maxDepth, visited, recursionStack);

        recursionStack.erase(dependency);
    }
}

void DependencyGraph::print(const std::string& root, int maxDepth) const {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> recursionStack;

    visited.insert(root);
    recursionStack.insert(root);

    std::cout << "\n=== Dependency Graph ===\n";
    std::cout << root << '\n';

    dfs(root, 0, maxDepth, visited, recursionStack);
}

void DependencyGraph::printReverse(const std::string& package, int maxDepth) const {
    Graph reverseGraph;

    for (const auto& [name, dependencies] : graph) {
        for (const auto& dependency : dependencies) {
            reverseGraph[dependency].push_back(name);
        }
    }

    DependencyGraph reverseDependencyGraph(reverseGraph);

    if (!reverseDependencyGraph.contains(package)) {
        std::cout << "\nNo reverse dependencies found for: "
                  << package << '\n';
        return;
    }

    std::cout << "\n=== Reverse Dependency Graph ===\n";
    reverseDependencyGraph.print(package, maxDepth);
}