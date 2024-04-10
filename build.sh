#!/bin/bash

# format first
find src/base -type f \( -name "*.h" -o -name "*.cpp" \) -print0 | while IFS= read -r -d $'\0' file; do
    echo "Formatting $file"
    clang-format -i -style=file $file
done
find src/net -type f \( -name "*.h" -o -name "*.cpp" \) -print0 | while IFS= read -r -d $'\0' file; do
    echo "Formatting $file"
    clang-format -i -style=file $file
done

# build
if [ ! -d "build" ]; then
    mkdir build
fi
cd build

BUILD_MODE=${1:-Debug}

if [[ "$BUILD_MODE" != "Debug" && "$BUILD_MODE" != "Release" ]]; then
    echo "Invalid build mode: $BUILD_MODE. Supported modes are Debug and Release."
    exit 1
fi

echo "Building in $BUILD_MODE mode"

if [ ! -d $BUILD_MODE ]; then
    mkdir $BUILD_MODE
fi

cd $BUILD_MODE
cmake -DCMAKE_BUILD_TYPE=$BUILD_MODE ../..
make

if [ $? -ne 0 ]; then
    echo "Build failed in $BUILD_MODE mode!"
    exit 1
fi

echo "Build completed successfully in $BUILD_MODE mode!"