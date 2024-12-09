#!/bin/bash

make
make coverage
rm -rf ./CMakeCache.txt
rm -rf ./cmake_install.cmake
rm -rf ./build/.cmake
rm -rf ./build/build
rm -rf ./build/CMakeCache.txt
rm -rf ./build/cmake_install.cmake