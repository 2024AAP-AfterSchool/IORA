#!/bin/bash

MEMORY_CHECK=OFF

for arg in "$@"; do
    case $arg in
        -m=*|--memory-check=*)
        MEMORY_CHECK="${arg#*=}"
        shift
        ;;
    esac
done

if [ "$MEMORY_CHECK" = "ON" ]; then
    cmake -DENABLE_MEMORY_CHECK=ON .
    ctest -V
elif [ "$MEMORY_CHECK" = "OFF" ]; then
    cmake -DENABLE_MEMORY_CHECK=OFF .
else
    cmake -DENABLE_MEMORY_CHECK=ON .
fi

make
make coverage
genhtml ./test/coverage.info -o ./test/coverage_report --title "IORA Test Coverage" --legend --keep-descriptions
rm -rf ./test/coverage.info
rm -rf ./CMakeCache.txt
rm -rf ./cmake_install.cmake
rm -rf ./build/.cmake
rm -rf ./build/build
rm -rf ./build/CMakeCache.txt
rm -rf ./build/cmake_install.cmake
rn -rf ./CTestTestfile.cmake
find . -name "*.gcda" -delete
# open ./test/coverage_report/index.html