@echo off

if "%VCPKG_ROOT%"=="" (
    echo.
    echo ERROR: VCPKG_ROOT is not set.
    echo.
    echo Example:
    echo set VCPKG_ROOT=C:\Users\YourName\vcpkg
    echo.
    exit /b 1
)

cmake -S . -B build ^
  -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows

cmake --build build --config Debug

echo.
echo Build completed.