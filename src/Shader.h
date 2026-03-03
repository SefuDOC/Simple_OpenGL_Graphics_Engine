#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const { glUseProgram(program); }
    GLuint ID() const { return program; }

private:
    std::string LoadSourceFromFile(const std::string& path);
    GLuint Compile(GLenum type, const std::string& source);

private:
    GLuint program = 0;
};