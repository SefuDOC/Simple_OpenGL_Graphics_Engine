#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::string& path)
{
    glGenTextures(1, &id); // creez un obiect de tip textura pe gpu si il asociez idului obiectului de tip Texture
    glBindTexture(GL_TEXTURE_2D, id); // basically , de acum in colo, toate setarile pt operatii de tip GL_TEXTURE_2D o sa fie aplicate pe aceasta textura.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // basically ce se intampla cand UV urile sunt in afara de (0,1)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // ce se intampla cand textura e miscorata
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // cand textura e marita

    int w, h, channels; // channels- 3 = RGB , 4 RGBA; w si h dimensiunile
    stbi_set_flip_vertically_on_load(true); // UV are 0,0 in stanga jos; majoritatea formatelor au 0,0 in stanga sus -> facem flip

    //citirea imaginii png/jpg folosimd biblioteca stb
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n";
        return;
    }

    GLenum format = (channels == 3 ? GL_RGB : GL_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data); // aici se pune textura pe gpu
        // 0 este nivelul mipmap pe care l incarcam; 0 e nivelul cu cea mai mare rezolutie
    glGenerateMipmap(GL_TEXTURE_2D); // generam celelalte nivelele 1024->512->256-> etc.

    stbi_image_free(data);
}