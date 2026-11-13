
#ifndef TEST_REPOSITORY_READER_H
#define TEST_REPOSITORY_READER_H

#include <string>
#include <unordered_map>
#include <vector>

class TestRepositoryReader {
public:
    static bool load(
        const std::string& path,
        std::unordered_map<std::string, std::vector<std::string>>& graph,
        std::string& error
    );
};

#endif
