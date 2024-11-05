#!/bin/bash

MEMORY_CHECK=ON

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
open ./test/coverage_report/index.html