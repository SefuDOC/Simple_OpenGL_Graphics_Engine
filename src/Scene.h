#pragma once
#include "SceneNode.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Frustum.h"
#include <unordered_map>


class Scene {
public:
    SceneNode root; // radacina (nu are mesh de obicei)

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& proj) {

        glm::mat4 VP = proj * view;
        Frustum frustum;
        frustum.ExtractPlanes(VP);
        DrawNode(&root, shader, frustum);
    }

    std::unordered_map<std::string, SceneNode*> nodeByName;

    void BuildNameIndex()
    {
        nodeByName.clear();
        BuildNameIndexRecursive(&root);
    }

    SceneNode* FindNodeByName(const std::string& name)
    {
        auto it = nodeByName.find(name);
        return (it == nodeByName.end()) ? nullptr : it->second;
    }

private:
    void DrawNode(SceneNode* node, Shader& shader, const Frustum& frustum) {

        // verificare pentru culling
        glm::mat4 model = node->GetWorldMatrix();
        bool cullNode = false;

        if (node->mesh) {
            glm::vec3 worldCenter = glm::vec3(
                node->GetWorldMatrix() * glm::vec4(node->mesh->GetBoundingCenter(), 1.0f)
            );
            float radius = node->mesh->GetBoundingRadius();

            if (!frustum.IsSphereInside(worldCenter, radius)) {
                cullNode = true;
            }
        }

        // daca nodeul are mesh si culling e fals => desenam
        if (!cullNode && node->mesh) {

            GLuint modelLoc = glGetUniformLocation(shader.ID(), "Model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            const Material& mat = node->mesh->GetMaterial();

            GLuint baseColorLoc = glGetUniformLocation(shader.ID(), "uBaseColor");
            glUniform3fv(baseColorLoc, 1, glm::value_ptr(mat.baseColor));

            GLuint hasMaterialLoc = glGetUniformLocation(shader.ID(), "hasMaterial");
            glUniform1i(hasMaterialLoc, mat.hasMaterial ? 1 : 0);

            if (mat.hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mat.diffuseTexture);
                glUniform1i(glGetUniformLocation(shader.ID(), "uDiffuseMap"), 0);
            }
            glUniform1i(glGetUniformLocation(shader.ID(), "hasTexture"), mat.hasTexture);           

            node->mesh->Draw();
        }

        // desenam recursiv copiii
        for (auto& child : node->children) {
            DrawNode(child.get(), shader, frustum);
        }
    }

    void BuildNameIndexRecursive(SceneNode* node)
    {
        if (!node) return;

        //numele trebuie sa fie diferite (lene)
        nodeByName[node->name] = node;

        for (auto& c : node->children)
            BuildNameIndexRecursive(c.get());
    }
};