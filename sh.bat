echo >/dev/null # >nul & GOTO WINDOWS & rem ^

if [ "$1" == "setup" ]; then
    (cd dep/glfw/; mkdir build; cd build; cmake .. -DBUILD_SHARED_LIBS=OFF; make)
    (cd dep/glew/; mkdir build; cd build; cmake .. -DBUILD_SHARED_LIBS=OFF; make)
    (cd dep/fmt/; mkdir build; cd build; cmake .. -DBUILD_SHARED_LIBS=OFF; make)

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
elif [ "$1" == "build" ]; then
    (cd build/; make -j4)
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

set "rootdir=%CD%"

if "%1" == "" (
    set flag=1==1
    set /p input="Ingrese la accion a realizar... ej. (download, setup, build o run): "
) else (
    set flag=1==0
    set input=%1
)

if "%input%" == "setup" (
    if exist "%rootdir%\dep\glfw" ( cd %rootdir%\dep\glfw
    ) else ( echo El directorio `%rootdir%\dep\glfw` no existe. )
    mkdir build 2>nul
    cd build
    call cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF
    call mingw32-make -j4
    echo ============================
    echo ===== GLFW CONFIGURADO =====
    echo ============================

    if exist "%rootdir%\dep\glew" ( cd %rootdir%\dep\glew
    ) else ( echo El directorio `%rootdir%\dep\glew` no existe. )
    mkdir build 2>nul
    cd build
    call cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF
    call mingw32-make -j4
    echo ============================
    echo ===== GLEW CONFIGURADO =====
    echo ============================

    if exist "%rootdir%\dep\fmt" ( cd %rootdir%\dep\fmt
    ) else ( echo El directorio `%rootdir%\dep\fmt` no existe. )
    mkdir build 2>nul
    cd build
    call cmake .. -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF
    call mingw32-make -j4
    echo ===========================
    echo ===== FMT CONFIGURADO =====
    echo ===========================
) else if "%input%" == "build" (
    cd build
    call mingw32-make -j4
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