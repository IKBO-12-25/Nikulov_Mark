#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "emulator.h"

using namespace std;

int main() {
    string vfsName = "Practice1";
    string input;

    cout << "Shell Emulator started" << endl;
    cout << "Type 'exit' to quit" << endl;

    while (true) {
        cout << vfsName << " > ";

        if (!getline(cin, input)) {
            break;
        }

        vector<string> arguments = parseCommand(input);

        if (arguments.empty()) {
            continue;
        }

        if (arguments[0] == "exit") {
            cout << "Exiting emulator..." << endl;
            break;
        }

        executeCommand(arguments);
    }

    return 0;
}