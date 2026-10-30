#include "parser.h"

#include <cstdlib>
#include <sstream>

using namespace std;

string expandVariables(const string& input) {
    string result;

    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '$') {
            string variable;

            i++;

            while (i < input.size() &&
                   (isalnum(input[i]) || input[i] == '_')) {
                variable += input[i];
                i++;
            }

            i--;

            if (!variable.empty()) {
                const char* value = getenv(variable.c_str());

                if (value != nullptr) {
                    result += value;
                }
            } else {
                result += '$';
            }
        } else {
            result += input[i];
        }
    }

    return result;
}

vector<string> parseCommand(const string& input) {
    vector<string> arguments;
    stringstream stream(expandVariables(input));

    string argument;

    while (stream >> argument) {
        arguments.push_back(argument);
    }

    return arguments;
}