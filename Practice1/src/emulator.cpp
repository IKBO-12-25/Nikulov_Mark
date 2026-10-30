#include "emulator.h"
#include "parser.h"

#include <iostream>
#include <fstream>

using namespace std;

void executeCommand(
    const vector<string>& arguments,
    VirtualFileSystem& vfs
) {
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

bool runStartupScript(
    const string& path,
    const string& vfsName,
    VirtualFileSystem& vfs
) {
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Cannot open startup script: " << path << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        size_t first = line.find_first_not_of(" \t\r");

        if (first == string::npos) {
            continue;
        }

        line = line.substr(first);

        if (line.rfind("//", 0) == 0) {
            continue;
        }

        cout << vfsName << " > " << line << endl;

        vector<string> arguments = parseCommand(line);

        if (arguments.empty()) {
            continue;
        }

        if (arguments[0] == "exit") {
            cout << "Exiting emulator..." << endl;
            return false;
        }

        executeCommand(arguments, vfs);
    }

    return true;
}