# <<<===== Cargador de Proyectos =====>>>

if [ "$1" == "download" ]; then
    echo ""
elif [ "$1" == "setup" ]; then
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
elif [ "$1" == "build" ]; then
    (cd build/; make -j4)
elif [ "$1" == "run" ]; then
    (cd Release/; ./Release.app)
else
    echo "Error '$1': solo se reconocen tres posibles inputs..."
    echo " > ./sh.bat download"
    echo " > ./sh.bat setup"
    echo " > ./sh.bat build"
    echo " > ./sh.bat run"
fi