#!/bin/bash

echo "=== Running Employee Records System Tests ==="
echo "Version 1.0"
echo "=============================================="

rm -rf build
mkdir build
cd build

cmake ..
make

echo -e "\n=== Test Results ==="
./test_runner

cd ..
