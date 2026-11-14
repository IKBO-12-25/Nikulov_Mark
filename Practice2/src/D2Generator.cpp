#include "D2Generator.h"

#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace {

std::string escapeLabel(const std::string& value) {
    std::string result;

    for (char ch : value) {
        if (ch == '"' || ch == '\\') {
            result += '\\';
        }
        result += ch;
    }

    return result;
}

class GraphWriter {
public:
    explicit GraphWriter(const DependencyGraph::Graph& graph)
        : graph(graph) {}

    std::string generate(const std::string& root, int maxDepth) {
        addNode(root);

        visited.insert(root);
        recursionStack.insert(root);

        dfs(root, 0, maxDepth);

        return output.str();
    }

private:
    const DependencyGraph::Graph& graph;

    std::map<std::string, std::string> nodeIds;
    std::set<std::string> visited;
    std::set<std::string> recursionStack;
    std::set<std::string> edges;

    std::ostringstream output;
    int nextId = 0;

    std::string addNode(const std::string& package) {
        auto it = nodeIds.find(package);

        if (it != nodeIds.end()) {
            return it->second;
        }

        std::string id = "node" + std::to_string(nextId++);

        nodeIds[package] = id;

        output << id << ": \""
               << escapeLabel(package)
               << "\"\n";

        return id;
    }

    void dfs(
        const std::string& package,
        int depth,
        int maxDepth
    ) {
        if (depth >= maxDepth) {
            return;
        }

        auto it = graph.find(package);

        if (it == graph.end()) {
            return;
        }

        std::vector<std::string> dependencies = it->second;
        std::sort(dependencies.begin(), dependencies.end());

        for (const auto& dependency : dependencies) {
            std::string from = addNode(package);
            std::string to = addNode(dependency);

            std::string edge = from + " -> " + to;

            if (edges.insert(edge).second) {
                output << edge << '\n';
            }

            if (recursionStack.count(dependency)) {
                continue;
            }

            if (visited.count(dependency)) {
                continue;
            }

            visited.insert(dependency);
            recursionStack.insert(dependency);

            dfs(dependency, depth + 1, maxDepth);

            recursionStack.erase(dependency);
        }
    }
};

}

std::string D2Generator::generate(
    const DependencyGraph::Graph& graph,
    const std::string& root,
    int maxDepth
) {
    GraphWriter writer(graph);
    return writer.generate(root, maxDepth);
}