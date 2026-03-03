#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>


struct Vertex {
    glm::vec3 position{ 0.0f };
    glm::vec3 normal{ 0.0f, 0.0f, 1.0f };
    glm::vec2 texCoord{ 0.0f };
};


struct Material {
    glm::vec3 baseColor{ 1.0f, 0.f, 1.0f };
    
    GLuint diffuseTexture = 0;
    bool hasTexture = false;
    

    bool hasMaterial = true;
};

class Mesh {
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material = {});
    ~Mesh();


    Mesh(const Mesh&) = delete;
    // definit contructorul care copiaza alt mesh dar il facem interzis sa evitam nebunii
    // nu putem crea un mesh din alt mesh
    
    Mesh& operator=(const Mesh&) = delete;
    // overload la "=" astfel incat daca facem a = b, unde a e mesh si b e mesh, sa zica compilatorul stop, potoleste te cu prostiile
    // nu putem face a = b pt meshuri
    
    // astea sunt pentru a evita sa avem doua meshuri ca aceleasi vao vbo si ebo
    
    Mesh(Mesh&& other) noexcept; // move constructor
    Mesh& operator=(Mesh&& other) noexcept; // move constructor

    void Draw() const;

    const Material& GetMaterial() const { return material; }

    glm::vec3 GetBoundingCenter() const { return boundingCenter; }
    float GetBoundingRadius() const { return boundingRadius; }

private:
    void CreateBuffers();
    glm::vec3 boundingCenter{ 0.0f };
    float boundingRadius = 0.0f;


private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
};