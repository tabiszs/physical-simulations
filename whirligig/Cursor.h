#pragma once
#include "Object.h"
#include "device.h"

class Cursor : public Object
{
public:
	Cursor(std::shared_ptr<Shader> shader)
	{
        minPt = -2.0f;
        maxPt = 7.0f;
        AddAxis(minPt, maxPt, 0.0f, 1.0f, 0.0f, 1.0f, {1.0f, 0.0f, 0.0f});
        AddAxis(0.0f, 1.0f, minPt, maxPt, 0.0f, 1.0f, {0.0f, 1.0f, 0.0f});
        AddAxis(0.0f, 1.0f, 0.0f, 1.0f, minPt, maxPt, {0.0f, 0.0f, 1.0f});
        this->shader = shader;
	}

    glm::mat4 ModelMatrix();
    glm::mat4 ModelMatrixQuat();
    void LoadMeshTo(std::shared_ptr<Device> device);
    void UpdateMeshTo(std::shared_ptr<Device> device);
    void DrawModelOn(std::shared_ptr<Device> device);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& euler_angles);
    void SetRotation(const glm::quat& quaternion);
    void ImproveShortestPath(const glm::vec3& euler_angles);
    void SetQuaternion(const glm::quat& euler_angles);
    void SetEulerAngles(const glm::quat& quaternion);
    
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 euler_angles = { 0.0f, 0.0f, 0.0f };
    glm::quat quaternion = { 1.0f, 0.0f, 0.0f, 0.0f }; // w, x, y, z
private:
    float xMin, yMin, zMin, xMax, yMax, zMax, minPt, maxPt;
    float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

    void AddAxis(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, glm::vec3 color);
};

