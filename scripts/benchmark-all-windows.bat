set SCRIPT_DIR=%~dp0
set PROJECT_ROOT=%SCRIPT_DIR%..
set BUILD_DIR=%PROJECT_ROOT%\build\benchmarks


cmake -S %PROJECT_ROOT% -B %BUILD_DIR%\release -DTMATH_BUILD_BENCHMARKS=ON && ^
cmake --build %BUILD_DIR%\release --config Release && ^
ctest --test-dir %BUILD_DIR%\release -C Release --output-on-failure
