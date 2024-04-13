#!/bin/bash

mkdir -p build2

cd build2

cmake -DCMAKE_BUILD_TYPE=Release ..

cmake --build . --config Release

cd ..