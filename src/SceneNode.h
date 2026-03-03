#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Mesh.h"

class SceneNode {
public:

    std::string name;
    std::string scriptPath;
    int scriptRef = -1;

    Transform transform;
    Mesh* mesh = nullptr;
    SceneNode* parent = nullptr;

    std::vector<std::unique_ptr<SceneNode>> children;

    SceneNode() = default;
    SceneNode(Mesh* m) : mesh(m) {}

    // creeaza un copil si il leaga la this
    SceneNode* CreateChild(Mesh* m = nullptr) {
        children.push_back(std::make_unique<SceneNode>(m));
        SceneNode* child = children.back().get();
        child->parent = this;
        return child;
    }

    glm::mat4 GetWorldMatrix() const {
        if (parent)
            return parent->GetWorldMatrix() * transform.GetMatrix();
        else
            return transform.GetMatrix();
    }
};