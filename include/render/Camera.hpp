#ifndef CAMERA_H
#define CAMERA_H

#include "Global.hpp"
#include "Shader.hpp"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

class Camera {
private:
	//=====>>> Variables Privadas
    int width, height;
    float speed, sensitivity;

    bool firstClick;

public:
    //=====>>> Variables Publicas
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 matrix;

    float aspect_ratio;
    float fov_deg;

    //=====>>> Constructor y Destructor
    Camera(const int width, const int height, const float fovDeg, const glm::vec3 position);
    ~Camera();

    //=====>>> Funciones
    void updateMatrix();
    void inputs();
};

#endif