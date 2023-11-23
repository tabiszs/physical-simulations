#pragma once
#include "Object.h"
#include "device.h"

class Cube : public Object
{
public:
    Cube() : Cube(1.0f) {}
    Cube(float size) : Cube(-size, size, -size, size, -size, size) {}
    Cube(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
    {
        SetBoundings(xMin, xMax, yMin, yMax, zMin, zMax);

        indices = {
            // Front
            4, 5,
            5, 7,
            7, 6,
            6, 4,

            // Back
            0, 1,
            1, 3,
            3, 2,
            2, 0,

            // Side
            0, 4,
            1, 5,
            2, 6,
            3, 7
        };
        shader = ShaderHolder::Get().boxShader;
    }

    virtual glm::mat4 ModelMatrix();
    void LoadMeshTo(std::shared_ptr<Device> device);
    void UpdateMeshTo(std::shared_ptr<Device> device);
    void DrawModelOn(std::shared_ptr<Device> device);
    void Update(float t)
    {
    }

protected:
    void SetBoundings(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
    float xMin, xMax, yMin, yMax, zMin, zMax;
    float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
};

