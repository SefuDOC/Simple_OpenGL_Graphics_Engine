#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

////// TEXTURES
uniform sampler2D uDiffuseMap;
uniform bool hasTexture;

uniform bool hasMaterial;
uniform vec3 uBaseColor;

////// LIGHTING
uniform vec3 viewPos;
uniform vec3 lightDirection; 
uniform vec3 lightColor;

void main()
{
    vec3 objColor;

    if (!hasMaterial) {
        objColor = uBaseColor;
    } else {
        if (hasTexture)
            objColor = texture(uDiffuseMap, TexCoords).rgb;
        else
            objColor = uBaseColor;
    }

    // normalizam
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDirection);
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V); // halfway vector intre ochi si lumina

    // ambient
    vec3 ambient = 0.2 * objColor;

    // diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * objColor * lightColor;

    // specular (Blinn Phong)
    float shininess = 32.0;
    float spec = pow(max(dot(N, H), 0.0), shininess);
    vec3 specular = 0.15 * spec * lightColor;

    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, 1.0);
}