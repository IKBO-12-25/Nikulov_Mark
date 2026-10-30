#include "vfs.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;
namespace fs = filesystem;

// Load directory recursively
bool VirtualFileSystem::loadDirectory(const fs::path& path, Entry& entry) {
    try {
        for (const auto& item : fs::directory_iterator(path)) {
            if (item.is_symlink()) {
                continue;
            }

            Entry child;
            child.name = item.path().filename().string();

            if (item.is_directory()) {
                child.isDirectory = true;

                if (!loadDirectory(item.path(), child)) {
                    return false;
                }
            }
            else if (item.is_regular_file()) {
                child.isDirectory = false;

                ifstream file(item.path(), ios::binary);

                if (!file.is_open()) {
                    cerr << "Error: Cannot open file: "
                         << item.path() << endl;
                    return false;
                }

                child.content = string(
                    istreambuf_iterator<char>(file),
                    istreambuf_iterator<char>()
                );
            }
            else {
                continue;
            }

            entry.children.push_back(child);
        }

        sort(entry.children.begin(), entry.children.end(),
             [](const Entry& a, const Entry& b) {
                 return a.name < b.name;
             });

        return true;
    }
    catch (const fs::filesystem_error& error) {
        cerr << "VFS error: " << error.what() << endl;
        return false;
    }
}

// Load physical directory into memory
bool VirtualFileSystem::load(const string& path) {
    root.name = fs::absolute(path).filename().string();

    if (root.name.empty()) {
        root.name = "VFS";
    }

    root.isDirectory = true;
    root.content.clear();
    root.children.clear();

    return loadDirectory(path, root);
}

// Find file or directory in VFS
const VirtualFileSystem::Entry* VirtualFileSystem::findEntry(
    const string& path
) const {
    if (path.empty() || path == ".") {
        return &root;
    }

    const Entry* current = &root;

    stringstream stream(path);
    string part;

    while (getline(stream, part, '/')) {
        if (part.empty() || part == ".") {
            continue;
        }

        if (part == "..") {
            return nullptr;
        }

        bool found = false;

        for (const Entry& child : current->children) {
            if (child.name == part) {
                current = &child;
                found = true;
                break;
            }
        }

        if (!found) {
            return nullptr;
        }
    }

    return current;
}

// Check if entry exists
bool VirtualFileSystem::exists(const string& path) const {
    return findEntry(path) != nullptr;
}

// Check if entry is directory
bool VirtualFileSystem::isDirectory(const string& path) const {
    const Entry* entry = findEntry(path);

    return entry != nullptr && entry->isDirectory;
}

// Read file content from memory
bool VirtualFileSystem::readFile(
    const string& path,
    string& content
) const {
    const Entry* entry = findEntry(path);

    if (entry == nullptr || entry->isDirectory) {
        return false;
    }

    content = entry->content;
    return true;
}

// Get directory contents
bool VirtualFileSystem::listDirectory(
    const string& path,
    vector<string>& entries
) const {
    const Entry* entry = findEntry(path);

    if (entry == nullptr || !entry->isDirectory) {
        return false;
    }

    entries.clear();

    for (const Entry& child : entry->children) {
        string name = child.name;

        if (child.isDirectory) {
            name += "/";
        }

        entries.push_back(name);
    }

    return true;
}

// Print VFS structure
void VirtualFileSystem::printEntry(const Entry& entry, int level) const {
    for (int i = 0; i < level; i++) {
        cout << "  ";
    }

    cout << (entry.isDirectory ? "[DIR] " : "[FILE] ");
    cout << entry.name << endl;

    for (const Entry& child : entry.children) {
        printEntry(child, level + 1);
    }
}

void VirtualFileSystem::print() const {
    printEntry(root, 0);
}