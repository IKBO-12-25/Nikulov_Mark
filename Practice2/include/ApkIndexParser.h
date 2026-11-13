
#ifndef APK_INDEX_PARSER_H
#define APK_INDEX_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

class ApkIndexParser {
public:
    using Graph = std::unordered_map<
        std::string,
        std::vector<std::string>
    >;

    static bool getDependencies(
        const std::string& indexPath,
        const std::string& packageName,
        std::vector<std::string>& dependencies,
        std::string& error
    );

    static bool loadGraph(
        const std::string& indexPath,
        Graph& graph,
        std::string& error
    );
};

#endif
