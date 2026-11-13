
#include "Config.h"
#include "ArgumentParser.h"
#include "RepositoryReader.h"
#include "ApkIndexParser.h"

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    Config config;

    if (!ArgumentParser::parse(argc, argv, config)) {
        return 1;
    }

    config.print();

    if (config.testMode) {
        std::cerr
            << "Error: Stage 2 requires a remote Alpine repository URL. "
            << "Test mode will be implemented in Stage 3.\n";

        return 1;
    }

    std::string indexPath;
    std::string error;

    if (!RepositoryReader::downloadIndex(
            config.repository,
            indexPath,
            error)) {

        std::cerr << "Error: " << error << '\n';
        return 1;
    }

    std::vector<std::string> dependencies;

    if (!ApkIndexParser::getDependencies(
            indexPath,
            config.packageName,
            dependencies,
            error)) {

        std::cerr << "Error: " << error << '\n';
        return 1;
    }

    std::cout << "\n=== Direct Dependencies ===\n";
    std::cout << "Package: " << config.packageName << '\n';

    if (dependencies.empty()) {
        std::cout << "No direct dependencies found.\n";
    }
    else {
        for (const auto& dependency : dependencies) {
            std::cout << "- " << dependency << '\n';
        }
    }

    std::cout << "\nTotal dependencies: "
              << dependencies.size() << '\n';

    return 0;
}
