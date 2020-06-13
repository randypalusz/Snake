#!/bin/bash

rm -rf build
mkdir -p build/clang
cd build/clang
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../..
make -j 16