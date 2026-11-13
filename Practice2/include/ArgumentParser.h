
#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include "Config.h"

class ArgumentParser {
public:
    static bool parse(
        int argc,
        char* argv[],
        Config& config
    );

private:
    static void printHelp();
};

#endif
