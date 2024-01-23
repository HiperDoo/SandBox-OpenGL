# =============================================
#          EDITA TUS PARAMETROS AQUI
# =============================================
# === Lado de CMake
SET(CMAKE_VERSION           3.10.0)
SET(CMAKE_PROJECT_NAME      "SandBox")
SET(CMAKE_PROJECT_VERSION   0.0.1)
SET(CMAKE_CXX_STANDARD      17)

# === Nombre del Ejecutable
IF(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release")
    SET(CMAKE_BUILD_TYPE Release)
    SET(EXECUTABLE_NAME     "Release")  # Nombre para el modo exe Release Mode
ELSE()
    SET(EXECUTABLE_NAME     "Debug")    # Nombre para el modo exe Debug Mode
ENDIF()

# === Banderas de Compilación
    # -g:               Genera información de debug.
    # -O0:              Deshabilita optimizaciones.

    # -s:               Elimina los símbolos de depuración (eliminar bandera al depurar código).
    # -O3:              Optimiza lo más que pueda.

    # -pipe:            Mejora los tiempos de compilación.
    # -Wall:            Habilita muchas de las advertencias disponibles.
    # -Wextra:          Habilita otras advertencias que -Wall no activa.
    # -Wpedantic:       Habilita más advertencias.
    # -Wfloat-equal:    Advierte si se usan comparaciones de igualdad (=) en variables de punto flotante.
    # -Wformat:         Advierte sobre el uso incorrecto de formato de texto (printf, scanf, etc).
    # -Winline:         Advierte si una función declarada como ‘inline’ no puede serlo.
    # -Wshadow:         Advierte la repetición de nombres para variables en diferentes niveles de scope.
    # -Wsign-conversion:    Advierte sobre las conversiones implícitas que pueden cambiar el signo.
SET(CMAKE_CXX_FLAGS
    "-s -O3"        # Modo Release
    #"-g -O0"       # Modo Debug
)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}\
    -pipe -Wall -Wextra -Wpedantic -Wfloat-equal -Wformat -Winline -Wshadow -Wsign-conversion\
    -march=core-avx2 -static-libgcc -static-libstdc++"
)

# === Banderas de compilación extra para Linux
SET(G_LINUX_CXX_FLAGS
    ""
)

# === Banderas de compilación extra para Windows
SET(G_WIN_CXX_FLAGS
    "-Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive"
    #-mwindows
)

# === Directorios donde se encuentran las librerías compiladas estáticamente
SET(LIB_FLAGS
    # Ejemplos de libs comunes (descomenta las que uses o agrega más).
    #${CMAKE_SOURCE_DIR}/dep/raylib/build/raylib/libraylib.a        # Raylib
    ${CMAKE_SOURCE_DIR}/dep/glfw/build/src/libglfw3.a              # GLFW
    ${CMAKE_SOURCE_DIR}/dep/glew/build/lib/libglew.a               # GLEW
    ${CMAKE_SOURCE_DIR}/dep/fmt/build/libfmt.a                     # FMT
)

# === Directorios donde se encuentran los encabezados de las librerías
SET(HEADER_FILES ${CMAKE_SOURCE_DIR}/dep
    # Ejemplos de libs comunes (descomenta las que uses o agrega más).
    #${CMAKE_SOURCE_DIR}/dep/raylib/src         # Raylib
    ${CMAKE_SOURCE_DIR}/dep/glfw/include       # GLFW
    ${CMAKE_SOURCE_DIR}/dep/glew/include       # GLEW
    ${CMAKE_SOURCE_DIR}/dep/glm                # GLM
    ${CMAKE_SOURCE_DIR}/dep/fmt/include        # FMT
    #${CMAKE_SOURCE_DIR}/dep/imgui               # IMGUI
    #${CMAKE_SOURCE_DIR}/dep/imgui/backends      # IMGUI
)
# ==================== FIN ====================



# =============================================
#                  FUNCIONES
# =============================================
# Función recursiva para archivos de encabezado
FUNCTION(HEADERS return search)
    FILE(GLOB_RECURSE list ${search})
    SET(dir_list "")
    FOREACH(file_path ${list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return} ${dir_list})
    SET(${return} ${${return}} PARENT_SCOPE)
ENDFUNCTION()


# =============================================
#                   INICIO
# =============================================
CMAKE_MINIMUM_REQUIRED(VERSION ${CMAKE_VERSION})
PROJECT(${CMAKE_PROJECT_NAME} VERSION ${CMAKE_PROJECT_VERSION})

SET(CMAKE_CXX_STANDARD_REQUIRED ON)

IF(UNIX AND NOT APPLE)
    SET(RELEASE "${EXECUTABLE_NAME}.app")
    SET(LIB_FLAGS ${LIB_FLAGS} GL)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${G_LINUX_CXX_FLAGS}")
ELSEIF(WIN32)
    SET(RELEASE ${EXECUTABLE_NAME})
    SET(LIB_FLAGS ${LIB_FLAGS} winmm.lib opengl32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${G_WIN_CXX_FLAGS}")
ENDIF()

# Obtención recursiva de archivos .cpp y .hpp del proyecto
FILE(GLOB_RECURSE SRC_FILES "src/*.cpp")
HEADERS(MY_HEADERS "include/*.hpp")
INCLUDE_DIRECTORIES(${HEADER_FILES} ${MY_HEADERS})

# Generación y ruta del ejecutable
ADD_EXECUTABLE(${RELEASE} ${SRC_FILES})
SET_TARGET_PROPERTIES(${RELEASE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Release")

TARGET_LINK_LIBRARIES(${RELEASE} ${LIB_FLAGS})

# Ignorar los warnings de main.cpp (deteccion de GPU dedicada)
SET_SOURCE_FILES_PROPERTIES(src/main.cpp PROPERTIES COMPILE_FLAGS "-w")


# Verifica que librerías dinámicas utiliza.
# Comando: ldd Release.app/.exe
#
# Identifica el tipo de archivo.
# Comando: file Release.app/.exe
#
# Obtén más información específica del ejecutable (linux).
# Comando readelf -<flag> Release.app