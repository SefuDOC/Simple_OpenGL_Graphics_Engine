#pragma once
#include <tinyxml2.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Scene.h"
#include "ObjLoader.h"

#include <unordered_map>

static std::unordered_map<std::string, Mesh*> meshCache; // retinem path <-> mesh pt  a nu crea de mai multe ori/ a pune de mai multe ori pe gpu

class SceneLoader {
public:

    // parsare scena xml
    static void LoadSceneFromXML(const std::string& path, Scene& scene)
    {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "ERROR: Could not load scene file: " << path << "\n";
            return;
        }

        tinyxml2::XMLElement* sceneElement = doc.FirstChildElement("scene");
        if (!sceneElement) {
            std::cerr << "ERROR: <scene> element missing in XML.\n";
            return;
        }

        // pentru fiecare <node> direct in <scene>
        for (tinyxml2::XMLElement* nodeElem = sceneElement->FirstChildElement("node"); nodeElem; nodeElem = nodeElem->NextSiblingElement("node"))
        {
            LoadNode(nodeElem, &scene.root);
        }
    }


private:

    static Mesh* LoadMeshCached(const std::string& path)
    {
        auto it = meshCache.find(path);

        if (it != meshCache.end()) // daca exista deja meshul, trimitem referinta
            return it->second;

        // daca nu exista, il cream acum
        Mesh* m = new Mesh(ObjLoader::Load(path));

        // salvam meshul in cache
        meshCache[path] = m;

        return m;
    }

    // functie recursiva pentru a incarca noduri
    static void LoadNode(tinyxml2::XMLElement* xmlNode, SceneNode* parent)
    {
        // cream nodul in scena, deocamdata nu are mesh
        SceneNode* node = parent->CreateChild(nullptr);

        //verificam daca are un model ( in xml ) si daca da , il cream si il asociam nodului
        const char* modelPath = xmlNode->Attribute("model");
        Mesh* meshPtr = nullptr;

        if (modelPath) {
            meshPtr = LoadMeshCached(modelPath);
            node->mesh = meshPtr;
        }

        //adaugam numele nodului si path to script
        const char* nameAttr = xmlNode->Attribute("name");
        if (nameAttr)
            node->name = nameAttr;
        else
            node->name = "Unnamed";

        const char* scriptAttr = xmlNode->Attribute("script");
        if (scriptAttr)
            node->scriptPath = scriptAttr;

        // pentru pozitie
        glm::vec3 pos(0);
        xmlNode->QueryFloatAttribute("posX", &pos.x);
        xmlNode->QueryFloatAttribute("posY", &pos.y);
        xmlNode->QueryFloatAttribute("posZ", &pos.z);
        node->transform.position = pos;

        // pentru rotatie
        glm::vec3 rot(0);
        xmlNode->QueryFloatAttribute("rotX", &rot.x);
        xmlNode->QueryFloatAttribute("rotY", &rot.y);
        xmlNode->QueryFloatAttribute("rotZ", &rot.z);
        node->transform.rotation = rot;

        // pentru scalare
        glm::vec3 scale(1);
        xmlNode->QueryFloatAttribute("scaleX", &scale.x);
        xmlNode->QueryFloatAttribute("scaleY", &scale.y);
        xmlNode->QueryFloatAttribute("scaleZ", &scale.z);
        node->transform.scale = scale;

        // luam fiecare "copil" node si reluam procesul
        for (tinyxml2::XMLElement* childXml = xmlNode->FirstChildElement("node"); childXml; childXml = childXml->NextSiblingElement("node"))
        {
            LoadNode(childXml, node);
        }
    }
};