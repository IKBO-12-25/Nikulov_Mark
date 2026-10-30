#!/bin/bash

cd "$(dirname "$0")/.." || exit 1

echo "Testing relative paths"

cmake -S . -B build
cmake --build build

./build/ShellEmulator \
    --vfs . \
    --script scripts/startup.txt