#include "Global.hpp"

/*namespace delta {
    double FPS{60.0f};
    double target_frame_time{1.0f / FPS};
    double elapsed_game_time{0.0f};
    double elapsed_win_title{0.0f};
    double lag_elapsed{0.0f};
    double sleep_time{0.0f};

    double default_frame_rate;
    uint32_t fps_counter{0};
    float dt{0.0f};

    std::chrono::high_resolution_clock::time_point current_time;
    std::chrono::high_resolution_clock::time_point previous_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time{0.0f};
    std::chrono::duration<double> frame_time{0.0f};

    void set_FPS(const FrameRate _fps) {
        if (_fps == FrameRate::VSYNC) FPS = default_frame_rate;
        else FPS = (double)_fps;

        target_frame_time = 1.0f / FPS;
    }

    void update_dt() {
        current_time = std::chrono::high_resolution_clock::now();
        elapsed_time = current_time - previous_time;
        previous_time = current_time;
        
        lag_elapsed += elapsed_time.count();
        elapsed_win_title += elapsed_time.count();
        dt = static_cast<float>(elapsed_time.count());

        fps_counter++;
        if (elapsed_win_title >= 1.0f) {
            sprintf(window::name + 22, "%.1f", (1.0f / elapsed_win_title) * fps_counter);
            glfwSetWindowTitle(window::glfw, window::name);

            elapsed_win_title = 0.0f;
            fps_counter = 0;
        }
    }

    void sleep_thread() {
        frame_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - current_time);
        sleep_time = target_frame_time - frame_time.count();

        if (sleep_time > 0.0f)
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
    }
};*/

namespace mouse {
    int32_t offsetX{0};
    int32_t offsetY{0};
    int32_t lastX{0};
    int32_t lastY{0};
    int32_t posX{0};
    int32_t posY{0};

    int32_t scrollX{0};
    int32_t scrollY{0};

    bool first_move{true};
};

WindowManager app;

io::Shader_Buffer shader_buff;
io::General_Buffer vertex_buff;
io::General_Buffer general_buff;