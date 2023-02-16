# =============================================
#                  FUNCIONES
# =============================================
# Funcion recursiva para archivos de encabezado
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

# GLFW - Configs
SET(OpenGL_GL_PREFERENCE LEGACY)
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# =============================================
#                   INICIO
# =============================================
CMAKE_MINIMUM_REQUIRED(VERSION 3.24.3)
PROJECT("SandBox" VERSION 0.1.0)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(LIB_FLAGS
    ${CMAKE_SOURCE_DIR}/dep/glfw/build/src/libglfw3.a
    ${CMAKE_SOURCE_DIR}/dep/glew/build/lib/libGLEW.a
    ${CMAKE_SOURCE_DIR}/dep/fmt/build/libfmt.a
)
IF (UNIX AND NOT APPLE)
    SET(RELEASE "Release.app")
    SET(LIB_FLAGS ${LIB_FLAGS} GL)
ELSEIF (WIN32)
    SET(RELEASE "Release")
ENDIF()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -std=c++17 -O3 -pipe -Wall -Wextra -Wpedantic -Wfloat-equal -march=core-avx2 \
    -DGLEW_STATIC -static-libgcc -static-libstdc++"
) # -static

# Obtencion recursiva de archivos .cpp y .h del proyecto
FILE(GLOB_RECURSE SRC_FILES "src/*.cpp")
HEADERS(HEADER_FILES "include/*.hpp")

ADD_EXECUTABLE(${RELEASE} ${SRC_FILES})
# Especificacion de la direccion de generacion del ejecutable
SET_TARGET_PROPERTIES(${RELEASE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Release")

# Porque esto hace que la ejecucion del .app sea mas veloz?
INCLUDE_DIRECTORIES(dep)
ADD_SUBDIRECTORY(dep/glfw)
ADD_SUBDIRECTORY(dep/glew/build/cmake)
ADD_SUBDIRECTORY(dep/glew)
ADD_SUBDIRECTORY(dep/glm)
ADD_SUBDIRECTORY(dep/fmt)

# Especificacion de a direccion de los archivos de encabezado
TARGET_INCLUDE_DIRECTORIES(${RELEASE}
    PUBLIC ${HEADER_FILES}
    PUBLIC "dep/glfw/include"
    PUBLIC "dep/glm"
    PUBLIC "dep/glew/include"
    PUBLIC "dep/stb"
    PUBLIC "dep/json"

    PUBLIC "dep/fmt/include"
)
TARGET_LINK_DIRECTORIES(${RELEASE} PRIVATE "dep/glfw/src")

TARGET_LINK_LIBRARIES(${RELEASE} PRIVATE ${LIB_FLAGS})

ADD_DEFINITIONS(${FLAGS})