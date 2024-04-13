#!/bin/bash

mkdir -p build2

cd build2 || exit

cmake ..

cmake --build . --config Release

cd ..
