#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

    glm::mat4 GetMatrix() const {
        glm::mat4 M(1.0f);
        M = glm::translate(M, position);
        M = glm::rotate(M, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        M = glm::rotate(M, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        M = glm::rotate(M, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        M = glm::scale(M, scale);
        return M;
    }
};