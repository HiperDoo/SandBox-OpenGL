#include "Camera.hpp"

//=====>>> Constructor y Destructor
Camera::Camera(const glm::vec3& pos, const GLfloat fovDeg)
    : worldUp{0.0f, 1.0f, 0.0f}, front{0.0f, 0.0f, 1.0f}, right{-1.0f, 0.0f, 0.0f}, up{0.0f, 1.0f, 0.0f},
    pitch(0), yaw(90), roll(0), speed(2.5f), sensitivity(100.0f), zoom(45.0f),
    aspect_ratio((GLfloat)(window::width) / (GLfloat)window::height), fov_deg(glm::radians(fovDeg)), first_click(GL_TRUE),
    projection(0.0f), view(0.0f), matrix(0.0f), position(pos) {}


Camera::~Camera() {}

//=====>>> Funciones
void Camera::updateAspectRatio() {
    aspect_ratio = (GLfloat)window::width / (GLfloat)window::height;
}

void Camera::inputs() {
    if ((glfwGetMouseButton(window::glfw, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) {
        glfwSetInputMode(window::glfw, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (first_click) {
            glfwSetCursorPos(window::glfw, window::half_width, window::half_height);
            first_click = GL_FALSE;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window::glfw, &mouseX, &mouseY);

        yaw   += static_cast<GLfloat>(mouseX - window::half_width) / window::width * sensitivity;
        pitch += static_cast<GLfloat>(window::half_height - mouseY) / window::height * sensitivity;

        pitch = pitch > 89.0f ? 89.0f : pitch;
        pitch = pitch < -89.0f ? -89.0f : pitch;

        yaw = yaw > 360.0f ? 0.0f : yaw;
        yaw = yaw < 0.0f ? 360.0f : yaw;
        
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));

        glfwSetCursorPos(window::glfw, window::half_width, window::half_height);
    } else if (glfwGetMouseButton(window::glfw, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window::glfw, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_click = GL_TRUE;
    }

    zoom -= mouse::scrollY;
    mouse::scrollY = 0;
    zoom = zoom < 1 ? 1 : zoom;
    zoom = zoom > 45 ? 45 : zoom;
    
    const float velocity{speed * delta::dt};
    if (glfwGetKey(window::glfw, GLFW_KEY_W) == GLFW_PRESS)             // Delante
        position += front * velocity;
    else if (glfwGetKey(window::glfw, GLFW_KEY_S) == GLFW_PRESS)        // Atrás
        position -= front * velocity;
        
    if (glfwGetKey(window::glfw, GLFW_KEY_D) == GLFW_PRESS)             // Derecha
        position += right * velocity;
    if (glfwGetKey(window::glfw, GLFW_KEY_A) == GLFW_PRESS)             // Izquierda
        position -= right * velocity;

    if (glfwGetKey(window::glfw, GLFW_KEY_SPACE) == GLFW_PRESS)         // Arriba
        position += up * velocity;
    if (glfwGetKey(window::glfw, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)    // Abajo
        position -= up * velocity;
    if (glfwGetKey(window::glfw, GLFW_KEY_TAB) == GLFW_PRESS)           // Rápido
        speed = 10.0f;
    else if (glfwGetKey(window::glfw, GLFW_KEY_D) == GLFW_RELEASE)      // Normal
        speed = 2.5f;
}

void Camera::updateMatrix() {
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(
        radians(zoom), aspect_ratio,
        NEAR_PLANE, FAR_PLANE
    );

    matrix = projection * view;
}