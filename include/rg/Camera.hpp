#ifndef MATF_RG_PROJEKAT_CAMERA_H
#define MATF_RG_PROJEKAT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rg {
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Camera {
        void updateCameraVectors();

    public:
        float fov = 45.f;
        float movementSpeed = 3.f;
        float yaw = -90.0f;
        float pitch = 0.0f;
        float mouseSensitivity = 0.2f;
        float zNear = 0.1f;
        float zFar = 100.0f;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 up{};
        glm::vec3 right{};
        glm::vec3 front = glm::vec3(0, 0, -1);

        Camera();

        explicit Camera(glm::vec3 position);

        glm::mat4 getViewMatrix() const;

        glm::mat4 getPerspectiveMatrix(float aspect) const;

        void move(Direction direction, float deltaTime);

        void rotate(float xoffset, float yoffset, bool constrainPitch);

        void zoom(float yoffset);
    };
}

#endif //MATF_RG_PROJEKAT_CAMERA_H
