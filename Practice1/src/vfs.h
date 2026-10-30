#ifndef VFS_H
#define VFS_H

#include <string>
#include <vector>
#include <filesystem>

using namespace std;

class VirtualFileSystem {
private:
    struct Entry {
        string name;
        bool isDirectory;
        string content;
        vector<Entry> children;
    };

    Entry root;

    bool loadDirectory(const filesystem::path& path, Entry& entry);
    void printEntry(const Entry& entry, int level) const;

    const Entry* findEntry(const string& path) const;
    Entry* findEntry(const string& path);

public:
    bool load(const string& path);
    void print() const;

    bool exists(const string& path) const;
    bool isDirectory(const string& path) const;
    bool readFile(const string& path, string& content) const;

    bool listDirectory(
        const string& path,
        vector<string>& entries
    ) const;

    bool remove(const string& path, bool recursive);
    bool copy(const string& source, const string& destination, bool recursive);
};

#endif