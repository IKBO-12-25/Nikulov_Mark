#!/bin/bash

cd "$(dirname "$0")/.." || exit 1

cmake --build build

./build/ShellEmulator \
    --vfs vfs/nested \
    --script scripts/startup.txt