#include "emulator.h"

#include <iostream>

using namespace std;

void executeCommand(const vector<string>& arguments) {
    if (arguments.empty()) {
        return;
    }

    string command = arguments[0];

    if (command == "ls") {
        cout << "Command: ls" << endl;

        for (int i = 1; i < arguments.size(); i++) {
            cout << "Argument: " << arguments[i] << endl;
        }
    }
    else if (command == "cd") {
        cout << "Command: cd" << endl;

        for (int i = 1; i < arguments.size(); i++) {
            cout << "Argument: " << arguments[i] << endl;
        }
    }
    else {
        cout << "Unknown command: " << command << endl;
    }
}