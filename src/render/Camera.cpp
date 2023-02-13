#include "Camera.hpp"

//=====>>> Constructor y Destructor
Camera::Camera(const int width, const int height, const float fovDeg, const glm::vec3 position)
    : width(width), height(height), speed(0.1f), sensitivity(100.0f), firstClick(true),
    position(position), orientation{0.0f, 0.0f, -1.0f}, up{0.0f, 1.0f, 0.0f}, view(1.0f),
    projection(1.0f), matrix(1.0f), aspect_ratio((float)width / height), fov_deg(glm::radians(fovDeg)) {}

Camera::~Camera() {}

//=====>>> Funciones
void Camera::updateMatrix() {
    view = glm::lookAt(position, position + orientation, up);
    projection = glm::perspective(
        fov_deg, aspect_ratio,
        NEAR_PLANE, FAR_PLANE
    );

    matrix = projection * view;
}

void Camera::inputs() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += speed * orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position += speed * -glm::normalize(glm::cross(orientation, up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position += speed * -orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += speed * glm::normalize(glm::cross(orientation, up));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += speed * up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        position += speed * -up;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        speed = 0.4f;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        speed = 0.1f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        glm::vec3 newOrientation = glm::rotate(
            orientation,
            glm::radians(-rotX),
            glm::normalize(glm::cross(orientation, up))
        );

        /*if (!((glm::angle(newOrientation, up) <= glm::radians(5.0f)) ||
            (glm::angle(newOrientation, -up) <= glm::radians(5.0f)))) {
            orientation = newOrientation;
        }*/
        if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			orientation = newOrientation;
		}

        orientation = glm::rotate(orientation, glm::radians(-rotY), up);

        glfwSetCursorPos(window, (width / 2), (height / 2));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}