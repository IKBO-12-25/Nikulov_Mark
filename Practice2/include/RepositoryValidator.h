
#ifndef REPOSITORY_VALIDATOR_H
#define REPOSITORY_VALIDATOR_H

#include <string>

class RepositoryValidator {
public:
    static bool isValidUrl(const std::string& url);

    static bool isValidTestFile(const std::string& path);
};

#endif
