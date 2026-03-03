#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>

std::string Shader::LoadSourceFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
// deschide fisierul, citeste tot si returneaza ca string;

GLuint Shader::Compile(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    //se creeaza un obiect gol de tip shader, generandu se idul respectiv

    const char* src = source.c_str();
    //opengl lucreaza cu pointeri, nu cu stringuri 

    glShaderSource(shader, 1, &src, nullptr);
    //se trimite efectiv textul la placa video si se asociaza acel cod cu acel id;
    //se trimite 1 sursa ( putea fi vector  de char* si atunci am fi putut avea mai multe surse)
    // nullptr e lungimea (nullptr -> pana la /0 e calculat de opengl)

    glCompileShader(shader);
    // ez compilare

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    //se verifica daca a avut succes compilarea

    if (!success) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "Shader compile error: " << log.data() << std::endl;
        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation failed");
    }
    // daca nu a avut succes -> debugging in log -> temele meu inca tin minte aceste debugguri
    return shader;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexCode = LoadSourceFromFile(vertexPath);
    std::string fragmentCode = LoadSourceFromFile(fragmentPath);

    GLuint vertex = Compile(GL_VERTEX_SHADER, vertexCode);
    GLuint fragment = Compile(GL_FRAGMENT_SHADER, fragmentCode);

    //se da load si se compileaza separat vertex si fragment shaderele

    program = glCreateProgram();
    //se creaza programul care le va contine pe amandoua
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    //ataseaza shaderele la program si le linkeaza
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetProgramInfoLog(program, len, nullptr, log.data());
        std::cerr << "Shader link error: " << log.data() << std::endl;
        glDeleteProgram(program);
        throw std::runtime_error("Shader link failed");
    }
    //daca linkarea esueaza -> debbuging in log

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    //nu mai sunt necesare shaderele deoarece ele au fost deja copiate in programul final
}

Shader::~Shader()
{
    if (program)
        glDeleteProgram(program);
}