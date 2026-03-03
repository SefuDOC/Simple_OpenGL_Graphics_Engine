#include "ObjLoader.h"
#include <tiny_obj_loader.h>
#include <iostream>
#include <stdexcept>
#include "Texture.h"

Mesh ObjLoader::Load(const std::string& filepath)
{
    std::string folder;
    
    size_t slash = filepath.find_last_of("/\\");   // caut folderul obiectului dat(importanta pt a gasi .mtl) ; stiu ca puteam da direct folderul dar na, meh!
    if (slash != std::string::npos)
        folder = filepath.substr(0, slash);
    
    //std::cout << folder << std::endl;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    // tinyobjloader retine datele din .obj si din .mtl in aceste structuri


    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(), folder.c_str());

    if (!warn.empty()) std::cout << "TinyObjLoader warning: " << warn << "\n";
    if (!ok) {
        std::cerr << "TinyObjLoader error: " << err << "\n";
        throw std::runtime_error("Failed to load .obj");
    }
    // ok e false daca nu se poate citi fisierul

    //pentru geometria modelului: pozitie, UV si normale;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned ind = 0;
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex v{};

            v.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.normal_index >= 0) {
                v.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) {
                v.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1] // eu inainte inversam; aparent, nu trebuie inversate :D
                };
            }

            vertices.push_back(v);
            indices.push_back(ind++);
        }
    }

    Material mat;

    if (materials.empty()) { // daca nu am gasit .mtl
        mat.hasMaterial = false;
    } else {
        mat.hasMaterial = true;
        auto& m = materials[0];

        mat.baseColor = { // in .mtl avem kd 1.0 0.2 0.3 care reprezinta culoarea difuza
            m.diffuse[0],
            m.diffuse[1],
            m.diffuse[2]
        };

        if (!m.diffuse_texname.empty()) { // daca am gasit o textura valida
            
            std::string texPath = folder + "/" + m.diffuse_texname;

            Texture tex;        
            tex = Texture(texPath);

            mat.diffuseTexture = tex.ID();
            mat.hasTexture = (mat.diffuseTexture != 0);
        }
               
    }

    return Mesh(vertices, indices, mat);
}