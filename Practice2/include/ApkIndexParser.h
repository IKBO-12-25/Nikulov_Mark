
#ifndef APK_INDEX_PARSER_H
#define APK_INDEX_PARSER_H

#include <string>
#include <vector>

class ApkIndexParser {
public:
    static bool getDependencies(
        const std::string& indexPath,
        const std::string& packageName,
        std::vector<std::string>& dependencies,
        std::string& error
    );
};

#endif
