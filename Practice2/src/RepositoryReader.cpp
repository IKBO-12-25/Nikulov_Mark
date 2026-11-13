
#include "RepositoryReader.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <system_error>

namespace fs = std::filesystem;

std::string RepositoryReader::shellQuote(
    const std::string& value
) {
    std::string result = "'";

    for (char ch : value) {
        if (ch == '\'') {
            result += "'\\''";
        }
        else {
            result += ch;
        }
    }

    result += "'";

    return result;
}

bool RepositoryReader::downloadIndex(
    const std::string& repository,
    std::string& indexPath,
    std::string& error
) {
    const std::string directory = "build/apk-index";
    const std::string archive = "build/APKINDEX.tar.gz";

    std::error_code ec;

    fs::create_directories(directory, ec);

    if (ec) {
        error = "Cannot create download directory: " + ec.message();
        return false;
    }

    std::string url = repository;

    while (!url.empty() && url.back() == '/') {
        url.pop_back();
    }

    url += "/APKINDEX.tar.gz";

    std::string downloadCommand =
        "curl -fL --silent --show-error "
        "--retry 2 --connect-timeout 15 --max-time 120 "
        "-o " + shellQuote(archive) +
        " " + shellQuote(url);

    std::cout << "Downloading APKINDEX...\n";

    if (std::system(downloadCommand.c_str()) != 0) {
        error = "Failed to download APKINDEX. Check URL and connection.";
        return false;
    }

    std::string extractCommand =
        "tar -xzf " + shellQuote(archive) +
        " -C " + shellQuote(directory);

    std::cout << "Extracting APKINDEX...\n";

    if (std::system(extractCommand.c_str()) != 0) {
        error = "Failed to extract APKINDEX.tar.gz";
        return false;
    }

    indexPath = directory + "/APKINDEX";

    if (!fs::exists(indexPath) ||
        !fs::is_regular_file(indexPath)) {
        error = "APKINDEX file was not found after extraction";
        return false;
    }

    std::cout << "APKINDEX successfully downloaded.\n";

    return true;
}
