#!/bin/bash

set -e

mkdir -p libs
cd libs

echo "Скачивание и сборка xlsxio"
git clone https://github.com/brechtsanders/xlsxio.git || true
cd xlsxio
mkdir -p build && cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(sysctl -n hw.ncpu)
sudo make install
cd ../../

echo "Библиотека xlsxio установлена."

echo "Скачивание и сборка libharu"
git clone https://github.com/libharu/libharu.git || true
cd libharu
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DLIBHPDF_ENABLE_PNG=ON -DLIBHPDF_SHARED=ON
make -j$(sysctl -n hw.ncpu)
sudo make install
cd ../../

echo "Библиотека libharu установлена."

cd ..

echo "Сборка проекта."
mkdir -p build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

echo "Бинарник собран"

echo "Копирование .dylib рядом с бинарником"
cp /usr/local/lib/libxlsxio_read.dylib .
cp /usr/local/lib/libhpdf.dylib .

echo "Запуск"
./test
