#include "Camera.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <cmath>

Camera::Camera(glm::vec3 pos)
    : position(pos), worldUp(0.0f, 1.0f, 0.0f),
    yaw(-90.0f), pitch(0.0f),
    speed(4.5f), sensitivity(0.1f)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Update(float deltaTime)
{
    if (!Input::IsFPSMode())
        return;

    ProcessKeyboard(deltaTime);
    ProcessMouse();
}

void Camera::ProcessKeyboard(float deltaTime)
{
    float velocity = speed * deltaTime;
    if (Input::IsKeyPressed(GLFW_KEY_W)) position += front * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_S)) position -= front * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_A)) position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_D)) position += glm::normalize(glm::cross(front, up)) * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_E)) position += up * velocity;
    if (Input::IsKeyPressed(GLFW_KEY_Q)) position -= up * velocity;
}

void Camera::ProcessMouse()
{
    double dx = Input::GetDeltaX();
    double dy = Input::GetDeltaY();

    dx *= sensitivity;
    dy *= sensitivity;

    yaw += static_cast<float>(dx);
    pitch += static_cast<float>(dy);

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, worldUp);
}
