@echo off
REM --------------------------------------------
REM 在 Native Tools CMD 下运行此脚本
REM --------------------------------------------

setlocal

set SCRIPT_DIR=%~dp0
set PROJECT_ROOT=%SCRIPT_DIR%..
set BUILD_DIR=%PROJECT_ROOT%\build

REM -------------------- MSVC --------------------
echo ==================================================
echo Building and testing with MSVC
echo ==================================================
cmake -S "%PROJECT_ROOT%" -B "%BUILD_DIR%\debug-msvc" -G "Ninja Multi-Config" ^
    -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl ^
    -DTMATH_BUILD_TESTS=ON
if errorlevel 1 (
    echo [ERROR] CMake configuration for MSVC failed!
    exit /b 1
)

cmake --build "%BUILD_DIR%\debug-msvc" --config Debug
if errorlevel 1 (
    echo [ERROR] Build for MSVC failed!
    exit /b 1
)

ctest --test-dir "%BUILD_DIR%\debug-msvc" -C Debug --output-on-failure
if errorlevel 1 (
    echo [ERROR] Tests for MSVC failed!
    exit /b 1
)

REM -------------------- MinGW --------------------
echo ==================================================
echo Building and testing with MinGW
echo ==================================================
cmake -S "%PROJECT_ROOT%" -B "%BUILD_DIR%\debug-mingw" -G "Ninja Multi-Config" ^
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ ^
    -DTMATH_BUILD_TESTS=ON
if errorlevel 1 (
    echo [ERROR] CMake configuration for MinGW failed!
    exit /b 1
)

cmake --build "%BUILD_DIR%\debug-mingw" --config Debug
if errorlevel 1 (
    echo [ERROR] Build for MinGW failed!
    exit /b 1
)

ctest --test-dir "%BUILD_DIR%\debug-mingw" -C Debug --output-on-failure
if errorlevel 1 (
    echo [ERROR] Tests for MinGW failed!
    exit /b 1
)

REM -------------------- Clang --------------------
echo ==================================================
echo Building and testing with Clang
echo ==================================================
cmake -S "%PROJECT_ROOT%" -B "%BUILD_DIR%\debug-clang" -G "Ninja Multi-Config" ^
    -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ^
    -DTMATH_BUILD_TESTS=ON
if errorlevel 1 (
    echo [ERROR] CMake configuration for Clang failed!
    exit /b 1
)

cmake --build "%BUILD_DIR%\debug-clang" --config Debug
if errorlevel 1 (
    echo [ERROR] Build for Clang failed!
    exit /b 1
)

ctest --test-dir "%BUILD_DIR%\debug-clang" -C Debug --output-on-failure
if errorlevel 1 (
    echo [ERROR] Tests for Clang failed!
    exit /b 1
)

echo ==================================================
echo All builds and tests passed successfully!
echo ==================================================

endlocal
exit /b 0
