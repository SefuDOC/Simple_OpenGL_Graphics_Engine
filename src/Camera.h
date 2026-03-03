#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 pos = { 0.0f, 0.0f, 3.0f });

    void Update(float deltaTime);
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const { return position; }

private:
    void ProcessKeyboard(float deltaTime);
    void ProcessMouse();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

};