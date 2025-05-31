#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

echo "Генерация CMake"
cmake .. || { echo "CMake завершился с ошибкой"; exit 1; }

echo "Сборка проекта"
cmake --build . || { echo "Сборка завершилась с ошибкой"; exit 1; }

echo "Запуск тестов через CTest"
export DYLD_LIBRARY_PATH="/usr/local/lib:$DYLD_LIBRARY_PATH"
ctest --output-on-failure
