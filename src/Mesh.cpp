#include "Mesh.h"
#include <stdexcept>

Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const Material& material)
    : vertices(vertices)
    , indices(indices)
    , material(material)
{
    glm::vec3 minPoint(1e9f);
    glm::vec3 maxPoint(-1e9f);

    for (auto& v : vertices) {
        minPoint.x = std::min(minPoint.x, v.position.x);
        minPoint.y = std::min(minPoint.y, v.position.y);
        minPoint.z = std::min(minPoint.z, v.position.z);

        maxPoint.x = std::max(maxPoint.x, v.position.x);
        maxPoint.y = std::max(maxPoint.y, v.position.y);
        maxPoint.z = std::max(maxPoint.z, v.position.z);
    }

    // centrul sferei
    boundingCenter = (minPoint + maxPoint) * 0.5f;

    // distanta maxima dintre centru si orice vertex
    float maxDist = 0.0f;
    for (auto& v : vertices) {
        float d = glm::length(v.position - boundingCenter);
        maxDist = std::max(maxDist, d);
    }
    boundingRadius = maxDist;
    CreateBuffers();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices))
    , indices(std::move(other.indices))
    , material(std::move(other.material))
    , vao(other.vao)
    , vbo(other.vbo)
    , ebo(other.ebo)
    , boundingCenter(other.boundingCenter)
    , boundingRadius(other.boundingRadius)
{
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
    other.boundingRadius = 0.0f;
    other.boundingCenter = glm::vec3(0.0f);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        if (ebo) glDeleteBuffers(1, &ebo);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (vao) glDeleteVertexArrays(1, &vao);

        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        material = std::move(other.material);

        vao = other.vao;
        vbo = other.vbo;
        ebo = other.ebo;

        boundingCenter = other.boundingCenter;
        boundingRadius = other.boundingRadius;

        other.vao = 0;
        other.vbo = 0;
        other.ebo = 0;
        other.boundingRadius = 0.0f;
        other.boundingCenter = glm::vec3(0.0f);
    }
    return *this;
}

Mesh::~Mesh() {
    if (ebo) glDeleteBuffers(1, &ebo);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

void Mesh::CreateBuffers() {

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // genereaza un obiect de fiecare tip

    glBindVertexArray(vao);
    // toate configuratiile de vertex si buffer se aplica la vao

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //legam vbo la arraybufferul curent (care in acest moment este vao)

    glBufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
        vertices.data(),
        GL_STATIC_DRAW);
    //alocam memorie ;(pe langa static , mai exista si dinamic si flow dar in cazul nostru nu cred ca vom face altceva :D
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //legam ebo la element bufferul curent( care in acest moment este vao)

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(),
        GL_STATIC_DRAW);
    //alocam memorie ;


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)) // primeam warning; cica asa e mai bine cu reinterpret_cast
    );
    //pentru position ,care are layout(location=0), trimitem 3 floaturi (xyz) si spunem unde gasim aceste date in tipul nostru de date vertex

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal))
    );
    //  normal layout(location=1)   
 
 
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, texCoord))
    );
    // textcoord layout(location=2) 


    glBindVertexArray(0);
    //se face unbind la vao (de acum incolo ,modificarile si configuratiile se aplica la "nimic"
}

void Mesh::Draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}