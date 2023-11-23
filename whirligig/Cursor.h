#pragma once
#include "Object.h"
#include "device.h"

class Cursor : public Object
{
public:
	Cursor()
	{
        minPt = -2.0f;
        maxPt = 7.0f;
        AddAxis(minPt, maxPt, 0.0f, 1.0f, 0.0f, 1.0f, {1.0f, 0.0f, 0.0f});
        AddAxis(0.0f, 1.0f, minPt, maxPt, 0.0f, 1.0f, {0.0f, 1.0f, 0.0f});
        AddAxis(0.0f, 1.0f, 0.0f, 1.0f, minPt, maxPt, {0.0f, 0.0f, 1.0f});
        shader = ShaderHolder::Get().cursorShader;
	}

    virtual glm::mat4 ModelMatrix();
    virtual void LoadMeshTo(std::shared_ptr<Device> device);
    virtual void UpdateMeshTo(std::shared_ptr<Device> device);
    virtual void DrawModelOn(std::shared_ptr<Device> device);
    virtual void Update(float t);

    float position[3] = { 0.0f, 0.0f, 0.0f };
    float euler_angles[3] = { 0.0f, 0.0f, 0.0f };
private:
    float xMin, yMin, zMin, xMax, yMax, zMax, minPt, maxPt;
    float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

    void AddAxis(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, glm::vec3 color);
};

