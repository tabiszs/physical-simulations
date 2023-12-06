#pragma once
#include "Object.h"
#include "device.h"
#include <array>

class Cube : public Object
{
public:
    Cube() {}
    Cube(float size) : Cube(0, size, 0, size, 0, size) {}
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
    virtual void LoadMeshTo(std::shared_ptr<Device> device);
    virtual void UpdateMeshTo(std::shared_ptr<Device> device);
    virtual void DrawModelOn(std::shared_ptr<Device> device);
    virtual void Update(float t) {}
    glm::vec3 GetDimensions();

protected:
    void SetBoundings(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
    float xMin, xMax, yMin, yMax, zMin, zMax;
    float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
};

