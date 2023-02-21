#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <chrono>
#include <exception>

#include "Log.hpp"
#include "Utilities.hpp"

extern GLFWwindow* window;
extern char* window_name;
extern int screen_width;
extern int screen_height;

extern io::File_Buffer<32 * ONE_MB> vertex_buff;
extern io::File_Buffer<16 * ONE_KB> shader_buff;

#endif