
#include "RepositoryValidator.h"

#include <filesystem>
#include <filesystem>
#include <string>

bool RepositoryValidator::isValidUrl(const std::string& url) {
    const std::string https = "https://";
    const std::string http = "http://";

    std::size_t position;

    if (url.rfind(https, 0) == 0) {
        position = https.size();
    }
    else if (url.rfind(http, 0) == 0) {
        position = http.size();
    }
    else {
        return false;
    }

    if (position >= url.size()) {
        return false;
    }

    std::size_t end = url.find('/', position);

    std::string host = url.substr(
        position,
        end == std::string::npos
            ? std::string::npos
            : end - position
    );

    return !host.empty() &&
           host.find_first_of(" \t\n") == std::string::npos;
}

bool RepositoryValidator::isValidTestFile(
    const std::string& path
) {
    namespace fs = std::filesystem;

    std::error_code error;

    return fs::exists(path, error) &&
           !error &&
           fs::is_regular_file(path, error) &&
           !error;
}
