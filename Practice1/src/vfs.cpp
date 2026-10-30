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

// Find mutable file or directory
VirtualFileSystem::Entry* VirtualFileSystem::findEntry(
    const string& path
) {
    if (path.empty() || path == ".") {
        return &root;
    }

    Entry* current = &root;

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

        for (Entry& child : current->children) {
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

// Remove file or directory from memory
bool VirtualFileSystem::remove(
    const string& path,
    bool recursive
) {
    size_t position = path.find_last_of('/');

    string parentPath;
    string name;

    if (position == string::npos) {
        name = path;
    }
    else {
        parentPath = path.substr(0, position);
        name = path.substr(position + 1);
    }

    if (name.empty() || name == "." || name == "..") {
        return false;
    }

    Entry* parent = findEntry(parentPath);

    if (parent == nullptr || !parent->isDirectory) {
        return false;
    }

    for (auto it = parent->children.begin();
         it != parent->children.end(); ++it) {

        if (it->name == name) {
            if (it->isDirectory &&
                !it->children.empty() &&
                !recursive) {
                return false;
            }

            parent->children.erase(it);
            return true;
        }
    }

    return false;
}

// Copy file or directory in memory
bool VirtualFileSystem::copy(
    const string& source,
    const string& destination,
    bool recursive
) {
    const Entry* sourceEntry = findEntry(source);

    if (sourceEntry == nullptr || sourceEntry == &root) {
        return false;
    }

    if (sourceEntry->isDirectory && !recursive) {
        return false;
    }

    size_t position = destination.find_last_of('/');

    string parentPath;
    string name;

    if (position == string::npos) {
        name = destination;
    }
    else {
        parentPath = destination.substr(0, position);
        name = destination.substr(position + 1);
    }

    if (name.empty() || name == "." || name == "..") {
        return false;
    }

    Entry* parent = findEntry(parentPath);

    if (parent == nullptr || !parent->isDirectory) {
        return false;
    }

    for (const Entry& child : parent->children) {
        if (child.name == name) {
            return false;
        }
    }

    Entry newEntry = *sourceEntry;
    newEntry.name = name;

    parent->children.push_back(newEntry);

    sort(parent->children.begin(), parent->children.end(),
         [](const Entry& a, const Entry& b) {
             return a.name < b.name;
         });

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