@echo off
cmake -DCMAKE_INSTALL_PREFIX="./out" -G "Visual Studio 17 2022" -A Win32 -B build --fresh
pause