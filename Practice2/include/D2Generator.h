#ifndef D2_GENERATOR_H
#define D2_GENERATOR_H

#include "DependencyGraph.h"
#include <string>

class D2Generator {
public:
    static std::string generate(
        const DependencyGraph::Graph& graph,
        const std::string& root,
        int maxDepth
    );
};

#endif