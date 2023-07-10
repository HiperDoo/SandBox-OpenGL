#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <exception>
#include <chrono>
#include <thread>

#include "Utilities.hpp"

namespace window {
    extern GLFWwindow* glfw;
    extern char* name;

    extern uint32_t width;
    extern uint32_t height;
    extern float half_width;
    extern float half_height;
};

namespace delta {
    extern double FPS;
    extern double target_frame_time;
    extern double elapsed_game_time;
    extern double elapsed_win_title;
    extern double lag_elapsed;
    extern double sleep_time;

    extern double default_frame_rate;
    extern uint32_t fps_counter;
    extern float dt;

    extern std::chrono::high_resolution_clock::time_point current_time;
    extern std::chrono::high_resolution_clock::time_point previous_time;
    extern std::chrono::duration<double> elapsed_time;
    extern std::chrono::duration<double> frame_time;

    enum FrameRate {
        VSYNC = 0,
        _30_FPS = 30,
        _45_FPS = 45,
        _50_FPS = 50,
        _60_FPS = 60,
        _75_FPS = 75,
        _90_FPS = 90,
        _120_FPS = 120,
        _200_FPS = 200,
        _240_FPS = 240,
        UNLIMITED = 99999
    };

    extern void set_FPS(const FrameRate fps);
    extern void update_dt();
    extern void sleep_thread();

    inline bool fixedUpdate() { return lag_elapsed >= (1.0f / 20.0f); } //target_frame_time
    inline void feedFixedUpdate() { lag_elapsed -= (1.0f / 20.0f); } //target_frame_time
};

namespace mouse {
    extern int32_t offsetX;
    extern int32_t offsetY;
    extern int32_t lastX;
    extern int32_t lastY;
    extern int32_t posX;
    extern int32_t posY;

    extern int32_t scrollX;
    extern int32_t scrollY;

    extern bool first_move;
};

extern io::Shader_Buffer shader_buff;
extern io::Vertex_Buffer vertex_buff;

#endif