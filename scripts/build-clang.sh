#!/bin/bash

rm -rf build
rm -f snake.cc
mkdir -p build/clang
cd build/clang
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../..
make -j 16
ln -sf build/clang/src/game.cc ../../snake.cc
chmod 777 ../../snake.cc
