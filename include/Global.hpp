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

#include "Utilities.hpp"

extern GLFWwindow* window;
extern char* window_name;
extern int screen_width;
extern int screen_height;

extern io::Shader_Buffer shader_buff;
extern io::Vertex_Buffer vertex_buff;

#endif