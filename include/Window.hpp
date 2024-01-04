#pragma once
#include "Global.hpp"

class WindowManager {
private:
    static WindowManager* window_manager;

private:
    // Variables para el manejo de fotogramas y DT
    // ===========================================
    double now_clk;
    double last_clk;
    double lag_elapsed;
    
    double title_fps_timer;
    uint32_t fps_counter;
    public: float dt;

public:
    // Variables para la ventana de OpenGL
    // ===================================
    GLFWwindow* glfw;

    char title[MAX_TITLE_SZIE + 32];
    uint32_t title_size;

    uint32_t width;
    uint32_t height;
    float half_width;
    float half_height;

    uint32_t screen_width;
    uint32_t screen_height;

private:
    // Variables para el manejo de la cámara
    // =====================================
    glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    // Las tres rotaciones que un objeto tridimensional tiene.
    GLfloat pitch, yaw, roll;

    GLfloat speed, sensitivity, zoom;
    GLfloat aspect_ratio, fov_deg;
    GLboolean first_click;
public:
    glm::mat4 projection, view; // Para el skybox
    glm::mat4 matrix;
    glm::vec3 position;

    // Variables para el control del mouse
    // ===================================
    int32_t offsetX, offsetY;
    int32_t lastX, lastY;
    int32_t posX, posY;
    int32_t scrollX, scrollY;

    bool first_move;

public:
    WindowManager();
    ~WindowManager();

    void window_init(const char* title, const uint32_t width, const uint32_t height);
    void window_update_clock();

    void camera_init(const glm::vec3& pos, const GLfloat fovDeg);
    void camera_inputs();
    void camera_update_Matrix();

    inline bool window_is_FixedUpdate() { return  lag_elapsed >= FIXED_TIME_UPDATE; }
    inline void window_feed_FixedUpdate() {     lag_elapsed -= FIXED_TIME_UPDATE; }

private:
    inline void UpdateFramebufferSize(GLsizei width, GLsizei height) {
        this->width = static_cast<uint32_t>(width);
        this->height = static_cast<uint32_t>(height);
        this->half_width = width / 2;
        this->half_height = height / 2;

        glViewport(0, 0, width, height);
        camera_update_AspectRatio();
    }

    inline void UpdateCursorPos(double pos_x, double pos_y) {
        posX = static_cast<int32_t>(pos_x);
        posY = static_cast<int32_t>(pos_y);

        if (first_move) {
            lastX = posX;
            lastY = posY;
            first_move = false;
        }

        offsetX = posX - lastX;
        offsetY = lastY - posY;

        lastX = posX;
        lastY = posY;
    }

    inline void camera_update_AspectRatio() { aspect_ratio = (GLfloat)width / (GLfloat)height; }

    inline float radians(const float degrees) {
        static constexpr float halfC{M_PI / 180.0f};
        return degrees * halfC;
    }

private:
    inline static void ErrorCallback(int, const char* err_str) {
        cmd::console_print(cmd::opengl, cmd::error, err_str);
        throw EXIT_FAILURE;
    }

    inline static void FramebufferSizeCallback(GLFWwindow*, GLsizei width, GLsizei height) {
        window_manager->UpdateFramebufferSize(width, height);
    }

    inline static void CursorPosCallback(GLFWwindow*, double pos_x, double pos_y) {
        window_manager->UpdateCursorPos(pos_x, pos_y);
    }
};