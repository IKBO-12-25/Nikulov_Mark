cmake_minimum_required(VERSION 3.16)

project(ShellEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ShellEmulator
    src/main.cpp
    src/parser.cpp
    src/emulator.cpp
    src/vfs.cpp
)

target_compile_features(ShellEmulator PRIVATE cxx_std_17)