#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "vfs.h"
#include "parser.h"
#include "emulator.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    string vfsPath;
    string scriptPath;

    // Read command line arguments
    for (int i = 1; i < argc; i++) {
        string argument = argv[i];

        if (argument == "--vfs" && i + 1 < argc) {
            vfsPath = argv[++i];
        }
        else if (argument == "--script" && i + 1 < argc) {
            scriptPath = argv[++i];
        }
        else {
            cerr << "Error: Unknown or incomplete argument: "
                 << argument << endl;
            return 1;
        }
    }

    // Check required arguments
    if (vfsPath.empty() || scriptPath.empty()) {
        cerr << "Usage: ShellEmulator --vfs <path> --script <path>"
             << endl;
        return 1;
    }

    // Validate paths
    if (!fs::exists(vfsPath) || !fs::is_directory(vfsPath)) {
        cerr << "Error: Invalid VFS directory: " << vfsPath << endl;
        return 1;
    }

    if (!fs::exists(scriptPath) || !fs::is_regular_file(scriptPath)) {
        cerr << "Error: Invalid startup script: " << scriptPath << endl;
        return 1;
    }

    string vfsName = fs::canonical(vfsPath).filename().string();

    if (vfsName.empty()) {
        vfsName = "VFS";
    }

    // Display configuration
    cout << "========== Configuration ==========" << endl;
    cout << "VFS path: " << fs::absolute(vfsPath) << endl;
    cout << "Startup script: " << fs::absolute(scriptPath) << endl;
    cout << "===================================" << endl;

    VirtualFileSystem vfs;

    if (!vfs.load(vfsPath)) {
        cerr << "Error: Failed to load VFS" << endl;
        return 1;
    }

    cout << "VFS loaded successfully!" << endl;
    vfs.print();

    cout << "Shell Emulator started" << endl;

    // Execute startup script
    if (!runStartupScript(scriptPath, vfsName, vfs)) {
        return 0;
    }

    string input;

    // Interactive REPL
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

        executeCommand(arguments, vfs);
    }

    return 0;
}