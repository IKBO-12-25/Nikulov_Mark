#!/bin/bash

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

echo "Testing absolute paths"

"$PROJECT_DIR/build/ShellEmulator" \
    --vfs "$PROJECT_DIR" \
    --script "$PROJECT_DIR/scripts/startup.txt"