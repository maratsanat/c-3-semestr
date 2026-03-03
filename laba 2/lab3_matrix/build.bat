@echo off
echo Building Matrix Lab...

if not exist build mkdir build
cd build

echo Configuring...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

echo Building...
mingw32-make

echo.
echo Build complete! Run with: .\build\bin\lab3.exe
cd ..
pause