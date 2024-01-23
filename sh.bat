echo >/dev/null # >nul & GOTO WINDOWS & rem ^

set -e 
set -o pipefail

cores=$(nproc)
if [ "$cores" -gt 5 ]; then
    ((cores -= 2))
fi

if [ "$1" == "setup" ]; then
    (cd dep/glfw/; mkdir -p build; cd build; cmake .. -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF; make -j$cores)
    (cd dep/glew/; mkdir -p build; cd build; cmake .. -DBUILD_SHARED_LIBS=OFF; make -j$cores)
    (cd dep/fmt/;  mkdir -p build; cd build; cmake .. -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DFMT_TEST=OFF; make -j$cores)

    (mkdir -p build/debug;   cd build/debug;   cmake ../../ -DCMAKE_BUILD_TYPE=Debug)
    (mkdir -p build/release; cd build/release; cmake ../../ -DCMAKE_BUILD_TYPE=Release)

elif [ "$1" == "build" ]; then
    (cd build/release/; make -j$cores)

elif [ "$1" == "run" ]; then
    (cd Release/; ./Release.app)

else
    echo "Error '$1': solo se reconocen tres posibles inputs..."
    echo " > ./sh.bat setup"
    echo " > ./sh.bat build"
    echo " > ./sh.bat run"
fi

exit 0

:WINDOWS
@echo off
cls

set "rootdir=%CD%"

set cores=%NUMBER_OF_PROCESSORS%
if %cores% GTR 5 set /a "cores=%cores%-2"

if "%1" == "" (
    set flag=1==1
    set /p input="Ingrese la accion a realizar... ej. (setup, build o run): "
) else (
    set flag=1==0
    set input=%1
)

if "%input%" == "setup" (
    (cd %rootdir%\dep\glfw) || exit /b 1
    (cd build 2> NUL || (mkdir build & cd build)) || exit /b 1
    (call cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF) || exit /b 1
    (call mingw32-make -j%cores%) || exit /b 1

    (cd %rootdir%\dep\glew) || exit /b 1
    (cd build 2> NUL || (mkdir build & cd build)) || exit /b 1
    (call cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF) || exit /b 1
    (call mingw32-make -j%cores%) || exit /b 1

    (cd %rootdir%\dep\fmt) || exit /b 1
    (cd build 2> NUL || (mkdir build & cd build)) || exit /b 1
    (call cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DFMT_TEST=OFF) || exit /b 1
    (call mingw32-make -j%cores%) || exit /b 1

    cd %rootdir%
    (cd build 2> NUL || (mkdir build & cd build)) || exit /b 1
    (cd debug 2> NUL || (mkdir debug & cd debug)) || exit /b 1
    (call cmake ../../ -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug) || exit /b 1

    cd %rootdir%\build
    (cd release 2> NUL || (mkdir release & cd release)) || exit /b 1
    (call cmake ../../ -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release) || exit /b 1

) else if "%input%" == "build" (
    (cd build/release/ && call mingw32-make -j%cores%) || exit /b 1

) else if "%input%" == "run" (
    cd Release
    call Release.exe
    exit

) else (
    echo "Error '%input%': solo se reconocen tres posibles inputs (setup, build, run)..."
)

if %flag% (
    pause
)