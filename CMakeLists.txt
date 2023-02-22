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
    ${CMAKE_SOURCE_DIR}/dep/glew/build/lib/libglew.a
    ${CMAKE_SOURCE_DIR}/dep/fmt/build/libfmt.a
)

IF (UNIX AND NOT APPLE)
    SET(RELEASE "Release.app")
    SET(LIB_FLAGS GL)
ELSEIF (WIN32)
    SET(RELEASE "Release")
    SET(LIB_FLAGS ${LIB_FLAGS} opengl32)
ENDIF()


# Obtencion recursiva de archivos .cpp y .hpp del proyecto
FILE(GLOB_RECURSE SRC_FILES "src/*.cpp")
HEADERS(HEADER_FILES "include/*.hpp")

# Generacion y ruta del ejecutable
ADD_EXECUTABLE(${RELEASE} ${SRC_FILES})
SET_TARGET_PROPERTIES(${RELEASE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Release")

ADD_COMPILE_DEFINITIONS(GLEW_STATIC)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -std=c++17 -O3 -s -pipe -Wall -Wextra -Wpedantic -Wfloat-equal -march=core-avx2 \
    -static-libgcc -static-libstdc++"
)

INCLUDE_DIRECTORIES(
    ${HEADER_FILES}
    ${CMAKE_SOURCE_DIR}/dep
    ${CMAKE_SOURCE_DIR}/dep/glfw/include
    ${CMAKE_SOURCE_DIR}/dep/glew/include
    ${CMAKE_SOURCE_DIR}/dep/glm
    ${CMAKE_SOURCE_DIR}/dep/fmt/include
)

TARGET_LINK_LIBRARIES(${RELEASE} ${LIB_FLAGS})