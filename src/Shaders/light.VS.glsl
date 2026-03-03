#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    FragPos = vec3(Model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(Model))) * aNormal; // important sa trimitem unde se afla in lume obiectul
    TexCoords = aTexCoords;

    gl_Position = Projection * View * vec4(FragPos, 1.0);
}