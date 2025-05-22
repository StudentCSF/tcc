#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
EXECUTABLE="$BUILD_DIR/my_pdf_tests"


mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

echo "Генерация CMake"
cmake .. || { echo "CMake завершился с ошибкой"; exit 1; }

echo "Сборка проекта"
make || { echo "Сборка завершилась с ошибкой"; exit 1; }

if [[ ! -f "$EXECUTABLE" ]]; then
    echo "Исполняемый файл $EXECUTABLE не найден"
    exit 1
fi

cd "$SCRIPT_DIR" || exit 1

export DYLD_LIBRARY_PATH="/usr/local/lib:$DYLD_LIBRARY_PATH"

echo "Запуск тестов"
"$EXECUTABLE"
