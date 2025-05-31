cmake -S . -B ./build -A x64 -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Release -D CMAKE_SHARED_LIBRARY=ON
cmake --build ./build --config Release -j8
ctest --test-dir build --output-on-failure
app\Release\TCC.exe