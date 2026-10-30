#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>
#include <vector>

void executeCommand(const std::vector<std::string>& arguments);

bool runStartupScript(const std::string& path, const std::string& vfsName);

#endif