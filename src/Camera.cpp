#include <rg/Camera.hpp>


namespace rg {
    void Camera::updateCameraVectors() {
        front = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                          sin(glm::radians(pitch)),
                          sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
//        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//        front.y = sin(glm::radians(pitch));
//        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    Camera::Camera() {
        updateCameraVectors();
    }

    Camera::Camera(glm::vec3 position) : position(position) {
        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera::getPerspectiveMatrix(float aspect) const {
        return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    }

    void Camera::move(Direction direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        switch (direction) {
            case FORWARD: {
                position += front * velocity;
            }
                break;
            case BACKWARD: {
                position -= front * velocity;
            }
                break;
            case LEFT: {
                position -= right * velocity;
            }
                break;
            case RIGHT: {
                position += right * velocity;
            }
                break;
        }
    }

    void Camera::rotate(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;
        if (constrainPitch) {
            pitch = rg::clamp(pitch, -89.0f, 89.0f);
        }

        updateCameraVectors();
    }

    void Camera::zoom(float yoffset) {
        fov -= yoffset;
        fov = rg::clamp(fov, 1.0f, 45.0f);
    }
}