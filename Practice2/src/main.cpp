#include "Config.h"
#include "ArgumentParser.h"

#include <iostream>

int main(int argc, char* argv[]) {
    Config config;

    if (!ArgumentParser::parse(argc, argv, config)) {
        return 1;
    }

    config.print();

    return 0;
}
