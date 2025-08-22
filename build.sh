#!/usr/bin/env bash

cmake -S . -B build

# cmake --build build -j$(nproc --ignore=4)
cmake --build build -j $(( $(nproc) - 4 ))