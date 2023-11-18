#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Object.h"
#include "device.h"
#include "WhirligigSimulator.h"

class Cube : public Object
{
public:
    Cube()
    {
        xMin = yMin = zMin = minSize;
        xMax = yMax = zMax = maxSize;
        vertices = {
                xMin, yMin, zMax, //0
                xMax, yMin, zMax, //1
                xMin, yMax, zMax, //2
                xMax, yMax, zMax, //3
                xMin, yMin, zMin, //4
                xMax, yMin, zMin, //5
                xMin, yMax, zMin, //6
                xMax, yMax, zMin  //7
        };
        indices = {
            4,1,2,
            7,4,6,

            1,4,5,
            7,5,4,

            4,2,6,
            1,0,4,

            2,0,1,
            3,5,7,

            5,3,1,
            3,7,6,

            3,6,2,
            3,2,1,

            3,4 // diagonal line
        };
        shader = ShaderHolder::Get().cubeShader;
        diagonal_shader = ShaderHolder::Get().diagonal_shader;
        SetProperties();
    }

    void SetProperties()
    {
        quaternion = glm::quat({ 0.0f, 0.0f, glm::radians(45.0f) }) * glm::quat({ 1,0,0,0 }); // obrot o 45st wokol osi z // przyklad z katami eulera
        quaternion = glm::angleAxis(std::atan2f(1.0f, std::sqrt(2.0f)), glm::vec3(-1.0f, 0.0f, 0.0f)) * quaternion; // obrot o atan(sqrt(2)) // przyklad - os & kat
        
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

        W = quaternion * w; // == conjugate(q) * w * q
    }

    void Update(float t)
    {
        auto dWdt = [this](glm::vec3 v) {
            auto IxWt = glm::cross((inertia_tensor * W*dt), W*dt);
            if (use_gravitation)
            {
                auto mc = quaternion * mass_center;
                auto r = mc - glm::vec3(0.0f, 0.0f, 0.0f);
                auto f = mass * g;
                n = glm::cross(r, f);
                N = glm::conjugate(quaternion) * n;
                IxWt += N;
            }
            Wt = inv_inertia_tensor * IxWt;
            return Wt;
        };

        auto dQdt = [this](glm::quat Q) {
            glm::quat Qt = 0.5f * Q*dt * W*dt;
            return Qt;
        };

        // RungeKutty4
        glm::vec3 k_0 = dWdt(W);
        glm::vec3 k_1 = dWdt(W + dt * 0.5f * k_0);
        glm::vec3 k_2 = dWdt(W + dt * 0.5f * k_1);
        glm::vec3 k_3 = dWdt(W + dt * 1.0f * k_2);
        auto newW = W + dt*(
            + b_j[0] * k_0
            + b_j[1] * k_1
            + b_j[2] * k_2
            + b_j[3] * k_3);

        // RungeKutty4
        glm::quat k_0q = dQdt(quaternion);
        glm::quat k_1q = dQdt(quaternion + dt * 0.5f * k_0q);
        glm::quat k_2q = dQdt(quaternion + dt * 0.5f * k_1q);
        glm::quat k_3q = dQdt(quaternion + dt * 1.0f * k_2q);
        auto newQ = dt*(
            + b_j[0] * k_0q
            + b_j[1] * k_1q
            + b_j[2] * k_2q
            + b_j[3] * k_3q);


        // mnozenie kwaternionow = zlozenie obrotow
        // mnozenie znormalizowanych wektorow = znormalizowany wektor
        quaternion = glm::normalize(newQ) * quaternion; 
        W = newW;
    }

    void LoadMeshTo(std::shared_ptr<Device> device);
    void UpdateMeshTo(std::shared_ptr<Device> device);
    void DrawModelOn(std::shared_ptr<Device> device);

    glm::mat4 ModelMatrix() { 
        auto to = glm::toMat4(quaternion);
        return glm::toMat4(quaternion) * Mat::identity();
    }

    glm::mat3 inertia_tensor, inv_inertia_tensor; // wzgledem poczatku ukladu wspolrzednych
    glm::quat quaternion = glm::quat(1, 0, 0, 0); // (cos(0), 0,0,0) // brak obrotu 
    const float hp = glm::half_pi<float>();
    glm::vec3 w = glm::vec3(hp, hp, hp); // pi/2 rad na sek
    glm::vec3 mass_center;
    float density = 1.0f;
    float mass;

    float color[4] = { 0.8f, 0.2f, 0.5f, 0.5f };
    bool visible = true;
    float dt = 0.01f;
    float maxSize = 1.0f;
    float inflection = 0.0f;
    int trajectory_length = 1000;
    bool use_gravitation = false;
    std::shared_ptr<Shader> diagonal_shader;
    float diagonal_color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
private:
    float minSize = 0.0f;
    float xMin, yMin, zMin, xMax, yMax, zMax;
    glm::vec3 n, N, W, Wt;
    glm::vec3 g = { 0, -9.81f, 0 };

    // Runge Kutty Coefficients
    glm::mat4 a_ij = glm::transpose(glm::mat4(
        0, 0, 0, 0,
        0.5f, 0, 0, 0,
        0, 0.5f, 0, 0,
        0, 0, 1.0f, 0
    ));
    glm::vec4 c_i = { 0, 0.5, 0.5, 1 };
    glm::vec4 b_j = { 1. / 6, 1. / 3, 1. / 3, 1. / 6 };
};


