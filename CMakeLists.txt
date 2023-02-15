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
    set(${return} ${${return}} PARENT_SCOPE)
ENDFUNCTION()

#GLFW Configs
set(OpenGL_GL_PREFERENCE LEGACY)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# =============================================
#                   INICIO
# =============================================
CMAKE_MINIMUM_REQUIRED(VERSION 3.24.3)
PROJECT("SandBox" VERSION 0.1.0)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
IF (UNIX AND NOT APPLE)
    SET(RELEASE "Release.app")
    SET(LIB_FLAGS GL glfw glew_s)
ELSEIF (WIN32)
    SET(RELEASE "Release")
    SET(LIB_FLAGS glfw glew_s)
ENDIF()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -pipe -Wall -Wextra -Wpedantic -Wfloat-equal -march=core-avx2")

# Obtencion recursiva de archivos .cpp y .h del proyecto
FILE(GLOB_RECURSE SRC_FILES "src/*.cpp")
HEADERS(HEADER_FILES "include/*.hpp")

ADD_EXECUTABLE(${RELEASE} ${SRC_FILES})
# Especificacion de la direccion de generacion del ejecutable
SET_TARGET_PROPERTIES(${RELEASE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Release")

INCLUDE_DIRECTORIES(external)
# Porque esto hace que la ejecucion del .app sea mas veloz?
ADD_SUBDIRECTORY("external/glfw")
ADD_SUBDIRECTORY("external/glm")
ADD_SUBDIRECTORY("external/glew/build/cmake")
ADD_SUBDIRECTORY("external/fmt")

# Especificacion de a direccion de los archivos de encabezado
TARGET_INCLUDE_DIRECTORIES(${RELEASE}
    PUBLIC ${HEADER_FILES}
    PUBLIC "external/glfw/include"
    PUBLIC "external/glm"
    PUBLIC "external/glew/include"
    PUBLIC "external/stb"
    PUBLIC "external/json"
)
TARGET_LINK_DIRECTORIES(${RELEASE} PRIVATE "external/glfw/src")

TARGET_LINK_LIBRARIES(${RELEASE} ${LIB_FLAGS} fmt::fmt)

ADD_DEFINITIONS(${FLAGS})