#!/bin/bash

rm -rf build
mkdir build
cd build

BUILD_MODE=${1:-Debug}

if [[ "$BUILD_MODE" != "Debug" && "$BUILD_MODE" != "Release" ]]; then
    echo "Invalid build mode: $BUILD_MODE. Supported modes are Debug and Release."
    exit 1
fi

echo "Building in $BUILD_MODE mode"
mkdir $BUILD_MODE
cd $BUILD_MODE
cmake -DCMAKE_BUILD_TYPE=$BUILD_MODE ../..
make

echo "Build completed successfully in $BUILD_MODE mode!"