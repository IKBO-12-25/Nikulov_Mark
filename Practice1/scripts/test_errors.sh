#!/bin/bash

cd "$(dirname "$0")/.." || exit 1

echo "Test 1: Missing arguments"
./build/ShellEmulator

echo ""
echo "Test 2: Invalid VFS path"
./build/ShellEmulator \
    --vfs ./wrong_directory \
    --script ./scripts/startup.txt

echo ""
echo "Test 3: Invalid script path"
./build/ShellEmulator \
    --vfs . \
    --script ./wrong_script.txt

echo ""
echo "All error tests completed"