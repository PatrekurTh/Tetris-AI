#!/bin/bash

mkdir -p build2

cd build2

cmake ..

cmake --build . --config Release

cd ..