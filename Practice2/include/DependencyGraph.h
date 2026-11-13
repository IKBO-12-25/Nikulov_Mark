
#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class DependencyGraph {
public:
    using Graph = std::unordered_map<
        std::string,
        std::vector<std::string>
    >;

    explicit DependencyGraph(const Graph& graph);

    bool contains(const std::string& package) const;

    void print(
        const std::string& root,
        int maxDepth
    ) const;

private:
    Graph graph;

    void dfs(
        const std::string& package,
        int depth,
        int maxDepth,
        std::unordered_set<std::string>& visited,
        std::unordered_set<std::string>& recursionStack
    ) const;
};

#endif
