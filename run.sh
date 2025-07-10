#!/bin/bash
set -e

mkdir -p build
cd build
cmake ..
make -j
./basic_test
