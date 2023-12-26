#pragma once
#include "Global.hpp"
#include "Shader.hpp"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 2000.0f

class Camera {
private:
    glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    // Las tres rotaciones que puede tener un
    // objeto tridimencional en su propio punto.
    GLfloat pitch, yaw, roll;

    GLfloat speed, sensitivity, zoom;
    GLfloat aspect_ratio, fov_deg;
    GLboolean first_click;

    inline float radians(const float degrees) {
        static constexpr float halfC = M_PI / 180.0f;
        return degrees * halfC;
    }

public:
    glm::mat4 projection, view; // Para el skybox
    glm::mat4 matrix;
    glm::vec3 position;

    /// NOTE: https://learnopengl.com/Getting-started/Camera
    Camera(const glm::vec3& pos, const GLfloat fovDeg);
    ~Camera();

    void updateAspectRatio();
    void inputs();
    void updateMatrix();
};