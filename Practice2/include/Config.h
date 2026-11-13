#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    std::string packageName;
    std::string repository;
    bool testMode = false;
    bool reverseDependencies = false;
    int maxDepth = 0;

    void print() const;
};

#endif