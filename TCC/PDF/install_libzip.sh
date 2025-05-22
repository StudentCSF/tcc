#!/bin/bash

set -e

LIBZIP_VERSION="1.10.1"
PROJECT_ROOT="$(pwd)"
EXTERNAL_DIR="$PROJECT_ROOT/external"
LIBZIP_DIR="$EXTERNAL_DIR/libzip"
BUILD_DIR="$LIBZIP_DIR/build"

echo "Установка зависимостей..."
#if command -v brew &> /dev/null; then
#    brew install cmake
#elif command -v apt &> /dev/null; then
#    sudo apt update
#    sudo apt install -y cmake make gcc pkg-config
#fi

mkdir -p "$LIBZIP_DIR"
cd "$EXTERNAL_DIR"

echo "Загрузка libzip-$LIBZIP_VERSION..."
curl -LO "https://libzip.org/download/libzip-$LIBZIP_VERSION.tar.gz"
tar -xf "libzip-$LIBZIP_VERSION.tar.gz"
mv "libzip-$LIBZIP_VERSION" "$LIBZIP_DIR/src"

echo "Сборка libzip локально..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ../src -DCMAKE_INSTALL_PREFIX="$LIBZIP_DIR/install" -DBUILD_SHARED_LIBS=OFF
cmake --build . --target install -- -j$(nproc)

echo "libzip собран в $LIBZIP_DIR/install"
