@echo off
setlocal enabledelayedexpansion

rem 检查cmake是否安装
where cmake >nul 2>nul
if !errorlevel! neq 0 (
    echo Error: cmake not found. Please install cmake first.
    echo Installation steps:
    echo 1. Download cmake from https://cmake.org/download/
    echo 2. Run the installer and select "Add CMake to the system PATH for all users"
    echo 3. Restart the terminal and try again
    exit /b 1
)

rem 检查是否有可用的C++编译器
set USE_MINGW=0

if !USE_MINGW! equ 0 (
    rem 检查vcpkg是否安装
    where vcpkg >nul 2>nul
    if !errorlevel! neq 0 (
        echo Error: vcpkg not found. Please install vcpkg first.
        echo Installation steps:
        echo 1. git clone https://github.com/microsoft/vcpkg.git
        echo 2. cd vcpkg
        echo 3. .\bootstrap-vcpkg.bat
        echo 4. Add vcpkg to PATH
        exit /b 1
    )

    rem 检查VCPKG_ROOT环境变量
    if not defined VCPKG_ROOT (
        echo Warning: VCPKG_ROOT environment variable not set.
        echo Please set VCPKG_ROOT to the vcpkg installation directory.
        echo Example: set VCPKG_ROOT=C:\vcpkg
        
        rem Try to detect vcpkg location
        for /f "tokens=*" %%i in ('where vcpkg') do set VCPKG_PATH=%%i
        if defined VCPKG_PATH (
            echo Detected vcpkg at: !VCPKG_PATH!
            for %%i in (!VCPKG_PATH!) do set VCPKG_ROOT=%%~dpi
            echo Set VCPKG_ROOT to: !VCPKG_ROOT!
        ) else (
            echo Error: Could not detect vcpkg location.
            exit /b 1
        )
    )

    echo Using vcpkg from: !VCPKG_ROOT!

    rem 安装依赖
echo Installing dependencies...
vcpkg install protobuf grpc yaml-cpp openssl --triplet x64-windows
if !errorlevel! neq 0 (
    echo Error: Failed to install dependencies.
    echo This usually happens when Visual Studio is not properly installed.
    echo Please make sure you have installed Visual Studio with C++ build tools.
    echo You can also try using MinGW for compilation.
    exit /b 1
)
)

rem 创建build目录
if not exist build mkdir build
cd build

rem 生成项目
echo Generating project...
if !USE_MINGW! equ 1 (
    cmake .. -G "MinGW Makefiles" -DUSE_SYSTEM_LIBS=ON
) else (
    cmake .. -A x64 -DCMAKE_TOOLCHAIN_FILE=!VCPKG_ROOT!scripts\buildsystems\vcpkg.cmake -DUSE_SYSTEM_LIBS=OFF
)

if !errorlevel! neq 0 (
    echo Error: Failed to generate project.
    echo.
    echo Possible solutions:
    if !USE_MINGW! equ 0 (
        echo 1. Make sure Visual Studio is properly installed with C++ build tools.
        echo 2. Try running vcpkg with --debug flag for more information.
        echo 3. Try using MinGW instead: install MinGW-w64 and run build.bat again.
    ) else (
        echo 1. Make sure MinGW is properly installed and in PATH.
        echo 2. Install dependencies using MSYS2:
        echo    pacman -S mingw-w64-x86_64-protobuf mingw-w64-x86_64-grpc mingw-w64-x86_64-yaml-cpp mingw-w64-x86_64-openssl
    )
    exit /b 1
)

rem 编译项目
echo Building project...
if !USE_MINGW! equ 1 (
    mingw32-make
) else (
    cmake --build . --config Release
)

if !errorlevel! neq 0 (
    echo Error: Failed to build project.
    echo Please check the error messages above and fix any issues.
    exit /b 1
)

rem 输出结果
if !USE_MINGW! equ 1 (
    echo Build completed successfully!
    echo Binary file: %~dp0bin/bidding_model_server.exe
) else (
    echo Build completed successfully!
    echo Binary file: %~dp0bin/Release/bidding_model_server.exe
)

endlocal