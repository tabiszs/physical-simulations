#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Object.h"
#include "device.h"

class Cube : public Object
{
public:
    Cube()
    {
        xMin = yMin = zMin = minSize;
        xMax = yMax = zMax = maxSize;
        vertices = {
    minSize,minSize,minSize, // triangle 1 : begin
    minSize,minSize, maxSize,
    minSize, maxSize, maxSize, // triangle 1 : end
    maxSize, maxSize,minSize, // triangle 2 : begin
    minSize,minSize,minSize,
    minSize, maxSize,minSize, // triangle 2 : end
    maxSize,minSize, maxSize,
    minSize,minSize,minSize,
    maxSize,minSize,minSize,
    maxSize, maxSize,minSize,
    maxSize,minSize,minSize,
    minSize,minSize,minSize,
    minSize,minSize,minSize,
    minSize, maxSize, maxSize,
    minSize, maxSize,minSize,
    maxSize,minSize, maxSize,
    minSize,minSize, maxSize,
    minSize,minSize,minSize,
    minSize, maxSize, maxSize,
    minSize,minSize, maxSize,
    maxSize,minSize, maxSize,
    maxSize, maxSize, maxSize,
    maxSize,minSize,minSize,
    maxSize, maxSize,minSize,
    maxSize,minSize,minSize,
    maxSize, maxSize, maxSize,
    maxSize,minSize, maxSize,
    maxSize, maxSize, maxSize,
    maxSize, maxSize,minSize,
    minSize, maxSize,minSize,
    maxSize, maxSize, maxSize,
    minSize, maxSize,minSize,
    minSize, maxSize, maxSize,
    maxSize, maxSize, maxSize,
    minSize, maxSize, maxSize,
    maxSize, minSize, maxSize
        };
        indices = {
            0,1,2,
            3,4,5,

            6,7,8,
            9,10,11,

            12,13,14,
            15,16,17,

            18,19,20,
            21,22,23,

            24,25,26,
            27,28,29,

            30,31,32,
            33,34,35
        };
        shader = ShaderHolder::Get().cubeShader;

        // poczatkowe ustawienie 
        quaternion = glm::quat({ 0.0f, 0.0f, glm::radians(45.0f) }) * quaternion; // obrot o 45st wokol osi z // przyklad z katami eulera
        quaternion = glm::angleAxis(std::atan2f(1, std::sqrt(2)), glm::vec3(-1.0f, 0.0f, 0.0f)) * quaternion; // obrot o atan(sqrt(2)) // przyklad - os & kat
   
        W = { 0,0,0 };
        UpdateProperties();
    }

    void UpdateProperties()
    {
        mass = density * (xMax - xMin) * (yMax - yMin) * (zMax - zMin);
        mass_center = density * 0.5f * glm::vec3((xMax * xMax - xMin * xMin), (yMax * yMax - yMin * yMin), (zMax * zMax - zMin * zMin)) / mass;

        float X2 = 1.0f / 3 * (xMax * xMax * xMax - xMin * xMin * xMin);
        float Y2 = 1.0f / 3 * (yMax * yMax * yMax - yMin * yMin * yMin);
        float Z2 = 1.0f / 3 * (zMax * zMax * zMax - zMin * zMin * zMin);
        float XY = 0.5f * (xMax * xMax - xMin * xMin) * 0.5f * (yMax * yMax - yMin * yMin);
        float XZ = 0.5f * (xMax * xMax - xMin * xMin) * 0.5f * (zMax * zMax - zMin * zMin);
        float YZ = 0.5f * (yMax * yMax - yMin * yMin) * 0.5f * (zMax * zMax - zMin * zMin);

        inertia_tensor = density * glm::transpose(glm::mat3(
            Y2 + Z2, -XY, -XZ,
            -XY, X2 + Z2, -YZ,
            -XZ, -YZ, X2 + Y2
        ));
        inv_inertia_tensor = glm::inverse(inertia_tensor);

        auto r = mass_center - glm::vec3(0.0f, 0.0f, 0.0f);
        auto f = mass * g;
        n = glm::cross(r, f);
        N = glm::conjugate(quaternion) * n;

        auto IxWt = N + glm::cross((inertia_tensor * W), W);
        Wt = inv_inertia_tensor * IxWt;

        
    }

    void Update()
    {


        auto rotQ = glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        quaternion = rotQ * quaternion;
    }

    void LoadMeshTo(std::shared_ptr<Device> device);
    void UpdateMeshTo(std::shared_ptr<Device> device);
    void DrawModelOn(std::shared_ptr<Device> device);

    glm::mat4 ModelMatrix() { 
        auto to = glm::toMat4(quaternion);
        return glm::toMat4(quaternion) * Mat::identity();
    }

    glm::mat3 inertia_tensor, inv_inertia_tensor; // wzgledem poczatku ukladu wspolrzednych
    float density = 1.0f;
    float mass;
    glm::vec3 mass_center;
    glm::quat quaternion = glm::quat(1, 0, 0, 0); // (cos(0), 0,0,0) // brak obrotu 
   

    float color[4] = { 0.8f, 0.2f, 0.5f, 0.5f };
    bool visible = true;
private:
    float maxSize = 1.0f;
    float minSize = 0.0f;
    float xMin, yMin, zMin, xMax, yMax, zMax;
    glm::vec3 n, N, W, Wt;
    glm::vec3 g = { 0, -9.81, 0 };
};


