# CHIP-8 Emulator

OpenGL_CMAKE_SKELETON from https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton

Running the application locally:
```
cmake -DCMAKE_TOOLCHAIN_FILE="C:\Users\suhas\AppData\Local\emsdk\upstream\emscripten\cmake\Modules\Platform\Emscripten.cmake" -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" . -B build -DCMAKE_PREFIX_PATH=.
cmake --build build
cd build
python -m http.server 8000 --directory build
```
visit: http://localhost:8000