#!/bin/bash

echo "=== Running tests ==="

rm -rf build
mkdir build
cd build
cmake ..
make

./test_runner

cd ..
