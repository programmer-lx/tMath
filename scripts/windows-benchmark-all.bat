@echo off
REM --------------------------------------------
REM 在 Native Tools CMD 下运行此脚本
REM --------------------------------------------


setlocal

set SCRIPT_DIR=%~dp0
set PROJECT_ROOT=%SCRIPT_DIR%..
set BUILD_DIR=%PROJECT_ROOT%\build\benchmarks


cmake -S %PROJECT_ROOT% -B %BUILD_DIR%\release-benchmark ^
-G "Ninja Multi-Config" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl ^
-DTMATH_BUILD_BENCHMARKS=ON && ^
cmake --build %BUILD_DIR%\release-benchmark --config Release && ^
ctest --test-dir %BUILD_DIR%\release-benchmark -C Release --output-on-failure

endlocal
