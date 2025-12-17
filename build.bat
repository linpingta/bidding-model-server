@echo off
setlocal enabledelayedexpansion

rem 检查cmake是否安装
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: cmake not found. Please install cmake first.
    echo Visit https://cmake.org/download/ for installation instructions.
    exit /b 1
)

rem 检查是否有可用的C++编译器
echo Checking for C++ compiler...
where cl.exe >nul 2>nul
if %errorlevel% neq 0 (
    where g++ >nul 2>nul
    if %errorlevel% neq 0 (
        echo Warning: No C++ compiler found. Please make sure you have Visual Studio or MinGW installed.
        echo Visit https://visualstudio.microsoft.com/ for Visual Studio installation.
        echo Visit https://www.mingw-w64.org/ for MinGW installation.
    )
)

rem 创建build目录
if not exist build mkdir build
cd build

rem 生成项目
echo Generating project...
cmake .. -A x64 -DUSE_SYSTEM_LIBS=ON
if %errorlevel% neq 0 (
    echo Error: Failed to generate project.
    echo.
    echo Possible solutions:
    1. Install dependencies manually and set the paths correctly
    2. Use Visual Studio with CMake support
    3. Configure CMake to find your dependencies
    echo.
    echo If you have vcpkg installed, make sure it's in your PATH or set VCPKG_ROOT environment variable.
    echo Example: set VCPKG_ROOT=C:\vcpkg
    echo Then run: cmake .. -A x64 -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
    exit /b 1
)

rem 编译项目
echo Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Error: Failed to build project.
    echo Please check the error messages above and fix any issues.
    exit /b 1
)

echo Build completed successfully!
echo Binary file: %~dp0bin/Release/bidding_model_server.exe

endlocal