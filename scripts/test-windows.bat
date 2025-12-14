set SCRIPT_DIR=%~dp0
set PROJECT_ROOT=%SCRIPT_DIR%..
set BUILD_DIR=%PROJECT_ROOT%\build


cmake -S %PROJECT_ROOT% -B %BUILD_DIR%\debug -DTMATH_BUILD_TESTS=ON && ^
cmake --build %BUILD_DIR%\debug --config Debug && ^
ctest --test-dir %BUILD_DIR%\debug -C Debug --output-on-failure


cmake -S %PROJECT_ROOT% -B %BUILD_DIR%\release -DTMATH_BUILD_TESTS=ON && ^
cmake --build %BUILD_DIR%\release --config Debug && ^
ctest --test-dir %BUILD_DIR%\release -C Debug --output-on-failure
