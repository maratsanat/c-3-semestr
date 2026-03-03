@echo off
echo Building Subvector C Lab...

if not exist build mkdir build
cd build

echo Configuring...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

echo Building...
mingw32-make

echo.
echo Build complete! Run with: .\build\bin\laba1.1.exe
cd ..
pause