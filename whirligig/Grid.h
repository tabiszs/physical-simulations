#pragma once
#include "Object.h"
#include "device.h"
class Grid :
    public Object
{
public: 
    Grid()
    {
        position = { -xSize / 2.0f, 0.0f, -zSize / 2.0f };
        shader = ShaderHolder::Get().gridShader;
        GenerateMesh();
    }

    glm::mat4 ModelMatrix();
    void LoadMeshTo(std::shared_ptr<Device> device);
    void DrawModelOn(std::shared_ptr<Device> device);

public:
    void GenerateMesh();

    glm::vec3 position{};
    static const int xSize = 100;
    static const int zSize = 100;
};

