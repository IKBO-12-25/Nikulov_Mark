
#include "Config.h"
#include "ArgumentParser.h"
#include "RepositoryReader.h"
#include "ApkIndexParser.h"
#include "DependencyGraph.h"
#include "TestRepositoryReader.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    Config config;

    if (!ArgumentParser::parse(argc, argv, config)) {
        return 1;
    }

    config.print();

    DependencyGraph::Graph graph;
    std::string error;

    if (config.testMode) {
        std::cout << "\nLoading test repository...\n";

        if (!TestRepositoryReader::load(
                config.repository,
                graph,
                error)) {
            std::cerr << "Error: " << error << '\n';
            return 1;
        }
    }
    else {
        std::string indexPath;

        if (!RepositoryReader::downloadIndex(
                config.repository,
                indexPath,
                error)) {
            std::cerr << "Error: " << error << '\n';
            return 1;
        }

        if (!ApkIndexParser::loadGraph(
                indexPath,
                graph,
                error)) {
            std::cerr << "Error: " << error << '\n';
            return 1;
        }
    }

    DependencyGraph dependencyGraph(graph);

    if (!dependencyGraph.contains(config.packageName)) {
        std::cerr << "Error: Package not found: "
                  << config.packageName << '\n';
        return 1;
    }

    dependencyGraph.print(
        config.packageName,
        config.maxDepth
    );

    return 0;
}
