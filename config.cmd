@echo off
cmake -DCMAKE_INSTALL_PREFIX="./out" -G "Visual Studio 17 2022" -B build -A Win32
pause