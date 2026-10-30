#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>
#include <vector>

#include "vfs.h"

void executeCommand(
    const std::vector<std::string>& arguments,
    VirtualFileSystem& vfs,
    std::string& currentDirectory
);

bool runStartupScript(
    const std::string& path,
    const std::string& vfsName,
    VirtualFileSystem& vfs,
    std::string& currentDirectory
);

#endif