@echo off
setlocal enabledelayedexpansion

rem 检查cmake是否安装
where cmake >nul 2>nul
if !errorlevel! neq 0 (
    echo Warning: cmake not found in PATH.
    echo Trying to detect cmake installation...
    
    rem 检查常见的cmake安装位置
    set CMAKE_FOUND=0
    
    rem 检查每个可能的cmake安装路径
    if exist "C:\Program Files\CMake\bin\cmake.exe" (
        echo Found cmake at: "C:\Program Files\CMake\bin\cmake.exe"
        set "PATH=!PATH!;C:\Program Files\CMake\bin"
        set CMAKE_FOUND=1
    ) else (
        if exist "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
            echo Found cmake at: "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
            set "PATH=!PATH!;C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
            set CMAKE_FOUND=1
        ) else (
            if exist "C:\Program Files\Microsoft Visual Studio\17\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
                echo Found cmake at: "C:\Program Files\Microsoft Visual Studio\17\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
                set "PATH=!PATH!;C:\Program Files\Microsoft Visual Studio\17\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
                set CMAKE_FOUND=1
            ) else (
                if exist "C:\Program Files (x86)\CMake\bin\cmake.exe" (
                    echo Found cmake at: "C:\Program Files (x86)\CMake\bin\cmake.exe"
                    set "PATH=!PATH!;C:\Program Files (x86)\CMake\bin"
                    set CMAKE_FOUND=1
                )
            )
        )
    )
    
    if !CMAKE_FOUND! equ 0 (
        echo Error: cmake not found. Please install cmake first.
        echo Installation steps:
        echo 1. Download cmake from https://cmake.org/download/
        echo 2. Run the installer and select "Add CMake to the system PATH for all users"
        echo 3. Restart the terminal and try again
        exit /b 1
    )
    
    echo Added cmake to PATH. Now continuing...
)

rem 检查是否有可用的C++编译器
set USE_MINGW=0
set SKIP_VCPKG=0

rem 检查命令行参数
for %%i in (%*) do (
    if /i "%%i"=="--skip-vcpkg" (
        set SKIP_VCPKG=1
    )
    if /i "%%i"=="--use-mingw" (
        set USE_MINGW=1
    )
)

echo Compiler settings:
if !USE_MINGW! equ 1 (
    echo - Using MinGW compiler
) else (
    echo - Using Visual Studio compiler
)

if !SKIP_VCPKG! equ 1 (
    echo - Skipping vcpkg dependency check
) else (
    echo - Using vcpkg for dependencies
)

if !USE_MINGW! equ 0 (
    if !SKIP_VCPKG! equ 0 (
        rem 检查vcpkg是否安装
        where vcpkg >nul 2>nul
        if !errorlevel! neq 0 (
            echo Error: vcpkg not found. Please install vcpkg first.
            echo Or use --skip-vcpkg flag to skip vcpkg check.
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
            echo You can also try using --skip-vcpkg flag or --use-mingw flag for compilation.
            exit /b 1
        )
    )
)

rem 创建build目录
if not exist build mkdir build
cd build

rem 生成项目
echo Generating project...
if !USE_MINGW! equ 1 (
    cmake .. -G "MinGW Makefiles" -DUSE_SYSTEM_LIBS=ON
    goto cmake_done
)

if !SKIP_VCPKG! equ 1 (
    echo Using system libraries (SKIP_VCPKG flag set)
    cmake .. -A x64 -DUSE_SYSTEM_LIBS=ON
    goto cmake_done
)

cmake .. -A x64 -DCMAKE_TOOLCHAIN_FILE=!VCPKG_ROOT!\scripts\buildsystems\vcpkg.cmake -DUSE_SYSTEM_LIBS=OFF

:cmake_done

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