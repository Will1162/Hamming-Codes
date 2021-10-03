@echo off
cls

set outputTitle=Hamming Codes


if exist "%outputTitle%.exe" (
    del "%outputTitle%.exe"
)

echo Building...
echo.

cd src
start /wait /b gcc main.c -o "../%outputTitle%"
cd ../

if not %errorlevel% == 0 (
    echo.
    echo An error has occured during compilation
    exit /b -1
) else (
    echo Done!
    echo Running...
    echo.
    echo === START ===
    "%outputTitle%.exe"
    echo === END ===
    echo.
)

echo Exited with code %errorlevel%