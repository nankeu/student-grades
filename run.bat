@echo off
REM Build and run on Windows with CMake + MinGW (the compiler that comes with Code::Blocks)
set PATH=C:\Program Files\CodeBlocks\MinGW\bin;%PATH%

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 goto error
cmake --build build
if errorlevel 1 goto error

build\students.exe
pause
exit /b 0

:error
echo Build failed.
pause
exit /b 1
