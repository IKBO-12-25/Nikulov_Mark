
#ifndef REPOSITORY_READER_H
#define REPOSITORY_READER_H

#include <string>

class RepositoryReader {
public:
    static bool downloadIndex(
        const std::string& repository,
        std::string& indexPath,
        std::string& error
    );

private:
    static std::string shellQuote(const std::string& value);
};

#endif
