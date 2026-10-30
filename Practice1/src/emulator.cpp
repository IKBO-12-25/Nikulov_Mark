#include "emulator.h"
#include "parser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

// Normalize virtual path
string normalizePath(const string& currentDirectory, const string& path) {
    string fullPath;

    if (path.empty() || path[0] != '/') {
        fullPath = currentDirectory;

        if (!fullPath.empty()) {
            fullPath += "/";
        }

        fullPath += path;
    }
    else {
        fullPath = path;
    }

    vector<string> parts;
    stringstream stream(fullPath);
    string part;

    while (getline(stream, part, '/')) {
        if (part.empty() || part == ".") {
            continue;
        }

        if (part == "..") {
            if (!parts.empty()) {
                parts.pop_back();
            }
        }
        else {
            parts.push_back(part);
        }
    }

    string result;

    for (const string& item : parts) {
        if (!result.empty()) {
            result += "/";
        }

        result += item;
    }

    return result;
}

// Execute command
void executeCommand(
    const vector<string>& arguments,
    VirtualFileSystem& vfs,
    string& currentDirectory
) {
    if (arguments.empty()) {
        return;
    }

    string command = arguments[0];

    // ls command
    if (command == "ls") {
        if (arguments.size() > 2) {
            cout << "ls: too many arguments" << endl;
            return;
        }

        string path = currentDirectory;

        if (arguments.size() == 2) {
            path = normalizePath(currentDirectory, arguments[1]);
        }

        vector<string> entries;

        if (!vfs.listDirectory(path, entries)) {
            cout << "ls: cannot access '" << path
                 << "': No such directory" << endl;
            return;
        }

        for (const string& entry : entries) {
            cout << entry << endl;
        }
    }

    // cd command
    else if (command == "cd") {
        if (arguments.size() > 2) {
            cout << "cd: too many arguments" << endl;
            return;
        }

        string path;

        if (arguments.size() == 1) {
            path = "";
        }
        else {
            path = normalizePath(currentDirectory, arguments[1]);
        }

        if (!vfs.isDirectory(path)) {
            cout << "cd: " << path
                 << ": No such directory" << endl;
            return;
        }

        currentDirectory = path;
    }

    // date command
    else if (command == "date") {
        if (arguments.size() > 1) {
            cout << "date: too many arguments" << endl;
            return;
        }

        time_t currentTime = time(nullptr);
        cout << ctime(&currentTime);
    }

    // head command
    else if (command == "head") {
        int numberOfLines = 10;
        string filePath;

        if (arguments.size() == 2) {
            filePath = arguments[1];
        }
        else if (arguments.size() == 4 && arguments[1] == "-n") {
            try {
                size_t position;
                numberOfLines = stoi(arguments[2], &position);

                if (position != arguments[2].size() ||
                    numberOfLines < 0) {
                    cout << "head: invalid number of lines" << endl;
                    return;
                }
            }
            catch (...) {
                cout << "head: invalid number of lines" << endl;
                return;
            }

            filePath = arguments[3];
        }
        else {
            cout << "Usage: head [-n number] <file>" << endl;
            return;
        }

        filePath = normalizePath(currentDirectory, filePath);

        string content;

        if (!vfs.readFile(filePath, content)) {
            cout << "head: cannot open '" << filePath
                 << "': No such file" << endl;
            return;
        }

        stringstream stream(content);
        string line;

        for (int i = 0; i < numberOfLines && getline(stream, line); i++) {
            cout << line << endl;
        }
    }

    // rm command
    else if (command == "rm") {
        bool recursive = false;
        string path;

        if (arguments.size() == 2) {
            path = arguments[1];
        }
        else if (arguments.size() == 3 && arguments[1] == "-r") {
            recursive = true;
            path = arguments[2];
        }
        else {
            cout << "Usage: rm [-r] <path>" << endl;
            return;
        }

        path = normalizePath(currentDirectory, path);

        if (!vfs.remove(path, recursive)) {
            cout << "rm: cannot remove '" << path
                 << "': Operation failed" << endl;
            return;
        }

        cout << "Removed: " << path << endl;
    }

    // cp command
    else if (command == "cp") {
        bool recursive = false;
        string source;
        string destination;

        if (arguments.size() == 3) {
            source = arguments[1];
            destination = arguments[2];
        }
        else if (arguments.size() == 4 && arguments[1] == "-r") {
            recursive = true;
            source = arguments[2];
            destination = arguments[3];
        }
        else {
            cout << "Usage: cp [-r] <source> <destination>" << endl;
            return;
        }

        source = normalizePath(currentDirectory, source);
        destination = normalizePath(currentDirectory, destination);

        if (!vfs.copy(source, destination, recursive)) {
            cout << "cp: cannot copy '" << source
                 << "' to '" << destination
                 << "': Operation failed" << endl;
            return;
        }

        cout << "Copied: " << source
             << " -> " << destination << endl;
    }

    else {
        cout << "Unknown command: " << command << endl;
    }
}

// Execute startup script
bool runStartupScript(
    const string& path,
    const string& vfsName,
    VirtualFileSystem& vfs,
    string& currentDirectory
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

        cout << vfsName;

        if (!currentDirectory.empty()) {
            cout << "/" << currentDirectory;
        }

        cout << " > " << line << endl;

        vector<string> arguments = parseCommand(line);

        if (arguments.empty()) {
            continue;
        }

        if (arguments[0] == "exit") {
            cout << "Exiting emulator..." << endl;
            return false;
        }

        executeCommand(arguments, vfs, currentDirectory);
    }

    return true;
}