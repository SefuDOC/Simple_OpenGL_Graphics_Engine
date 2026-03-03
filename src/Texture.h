#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    Texture() = default;
    Texture(const std::string& path);

    GLuint ID() const { return id; }

private:
    GLuint id = 0;
};