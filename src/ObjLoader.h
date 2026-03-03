#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

class ObjLoader {
public:
    static Mesh Load(const std::string& filepath);
};