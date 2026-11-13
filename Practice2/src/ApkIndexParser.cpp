
#include "ApkIndexParser.h"

#include <fstream>
#include <sstream>
#include <iostream>

bool ApkIndexParser::getDependencies(
    const std::string& indexPath,
    const std::string& packageName,
    std::vector<std::string>& dependencies,
    std::string& error
) {
    std::ifstream file(indexPath);

    if (!file.is_open()) {
        error = "Cannot open APKINDEX file";
        return false;
    }

    std::string line;
    std::string currentPackage;

    std::vector<std::string> currentDependencies;

    bool packageFound = false;
    bool hasPackageRecords = false;

    dependencies.clear();

    auto finishRecord = [&]() {
        if (!currentPackage.empty()) {
            hasPackageRecords = true;
        }

        if (currentPackage == packageName) {
            dependencies = currentDependencies;
            packageFound = true;
        }

        currentPackage.clear();
        currentDependencies.clear();
    };

    while (std::getline(file, line)) {

        if (line.empty() || line == "\r") {
            finishRecord();
            continue;
        }

        // Remove carriage return if present.
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.rfind("P:", 0) == 0) {
            currentPackage = line.substr(2);
        }
        else if (line.rfind("D:", 0) == 0) {
            std::istringstream stream(line.substr(2));

            std::string dependency;

            while (stream >> dependency) {
                currentDependencies.push_back(dependency);
            }
        }
    }

    finishRecord();

    if (file.bad()) {
        error = "Error while reading APKINDEX";
        return false;
    }

    if (!hasPackageRecords) {
        error = "Invalid APKINDEX: no package records found";
        return false;
    }

    if (!packageFound) {
        error = "Package not found: " + packageName;
        return false;
    }

    return true;
}
