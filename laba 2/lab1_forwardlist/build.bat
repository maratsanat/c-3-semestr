@echo off
echo Building ForwardList Lab...

if not exist build mkdir build
cd build

echo Configuring...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

echo Building...
mingw32-make

echo.
echo Build complete! Run with: .\build\bin\lab1.exe
cd ..
pause