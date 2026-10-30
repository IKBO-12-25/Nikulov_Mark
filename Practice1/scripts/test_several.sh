#!/bin/bash

cd "$(dirname "$0")/.." || exit 1

cmake --build build

./build/ShellEmulator \
    --vfs vfs/several \
    --script scripts/startup.txt