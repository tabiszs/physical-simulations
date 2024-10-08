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

class WhirligigCube : public Object
{
public:
    WhirligigCube()
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
            4,1,2, // triangles
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
        SetQuaternion();

        mass = density * (xMax - xMin) * (yMax - yMin) * (zMax - zMin);
        mass_center = glm::vec3(0, sqrt(3) / 2, 0);

        float X2 = 1.0f / 3 * (xMax * xMax * xMax - xMin * xMin * xMin);
        float Y2 = 1.0f / 3 * (yMax * yMax * yMax - yMin * yMin * yMin);
        float Z2 = 1.0f / 3 * (zMax * zMax * zMax - zMin * zMin * zMin);
        float XY = 0.5f * (xMax * xMax - xMin * xMin) * 0.5f * (yMax * yMax - yMin * yMin);
        float XZ = 0.5f * (xMax * xMax - xMin * xMin) * 0.5f * (zMax * zMax - zMin * zMin);
        float YZ = 0.5f * (yMax * yMax - yMin * yMin) * 0.5f * (zMax * zMax - zMin * zMin);

        // version 1 - implicit
        inertia_tensor = density * glm::mat3(
            0.9167, 0, 0,
            0, 0.1667, 0,
            0, 0, 0.9167
        );
        inv_inertia_tensor = density * glm::mat3(
            1.0f / 0.9167, 0,0,
            0, 1.0f / 0.1667, 0,
            0,0, 1.0f / 0.9167
        );

        // version 2 - transformation to axis on main diagonal: I_transformed = R * I conj(R)
        //inertia_tensor = density * glm::toMat4(frameQ) * glm::mat4(
        //    Y2 + Z2, -XY, -XZ, 0,
        //    -XY, X2 + Z2, -YZ, 0,
        //    -XZ, -YZ, X2 + Y2, 0,
        //    0, 0, 0, 1
        //) * glm::toMat4(glm::conjugate(frameQ));

        //inv_inertia_tensor = density * glm::toMat4(frameQ) * glm::mat4(
        //    1.0f/(Y2 + Z2), -1.0f/XY, -1.0f/XZ, 0,
        //    -1.0f/XY, 1.0f/(X2 + Z2), -1.0f/YZ, 0,
        //    -1.0f/XZ, -1.0f/YZ, 1.0f/(X2 + Y2), 0,
        //    0,          0,          0,          1
        //) * glm::toMat4(glm::conjugate(frameQ));

        W = glm::vec3(0, velocity, 0);
    }

    void SetQuaternion()
    {
        frameQ = glm::quat({ 0.0f, 0.0f, glm::radians(45.0f) }); // obrot o 45st wokol osi z // przyklad z katami eulera
        frameQ = glm::angleAxis(std::atan2f(1.0f, std::sqrt(2.0f)), glm::vec3(-1.0f, 0.0f, 0.0f)) * frameQ; // obrot o atan(sqrt(2)) // przyklad - os & kat
        
        Q = glm::quat(1, 0, 0, 0);
        Q = glm::angleAxis(inflection, glm::vec3(0.0f, 0.0f, -1.0f)) * Q;
        need_update = true;
    }

    void Update(float t)
    {
        auto dWdt = [this](glm::quat Q, glm::vec3 W) {
            auto IxWt = - glm::cross((inertia_tensor * W), W);
            if (use_gravitation)
            {
                auto G = glm::conjugate(Q) * glm::quat(0,g) * Q;
                glm::vec3 F = mass * glm::vec3(G.x,G.y,G.z);
                glm::vec3 R = mass_center - glm::vec3(0.0f, 0.0f, 0.0f);
                N = glm::cross(R, F);
                IxWt += N;
            }
            Wt = inv_inertia_tensor * IxWt;
            return Wt;
        };

        auto dQdt = [this](glm::quat Q, glm::vec3 W) {
            glm::quat Qt = 0.5f * Q * glm::quat(0,W);
            return Qt;
        };

        // RungeKutty4
        glm::vec3 k0_w = dt * dWdt(Q,W);
        glm::quat k0_q = dt * dQdt(Q,W);

        glm::vec3 k1_w = dt * dWdt(Q + k0_q / 2.0f, W + k0_w / 2.0f);
        glm::quat k1_q = dt * dQdt(Q + k0_q / 2.0f, W + k0_w / 2.0f);

        glm::vec3 k2_w = dt * dWdt(Q + k1_q / 2.0f, W + k1_w / 2.0f);
        glm::quat k2_q = dt * dQdt(Q + k1_q / 2.0f, W + k1_w / 2.0f);

        glm::vec3 k3_w = dt * dWdt(Q + k2_q, W + k2_w);
        glm::quat k3_q = dt * dQdt(Q + k2_q, W + k2_w);

        auto Wt = (k0_w + 2.0f * k1_w + 2.0f * k2_w + k3_w) / 6.0f;
        auto Qt = (k0_q + 2.0f * k1_q + 2.0f * k2_q + k3_q) / 6.0f;

        Q = glm::normalize(Q + Qt);
        W += Wt;
        need_update = true;
    }

    glm::vec3 GetConeCoordinates()
    {
        return ModelMatrix() * glm::vec4(xMax, yMax, zMax, 1.0f);
    }

    glm::mat4 ModelMatrix() {
        return glm::toMat4(Q) * glm::toMat4(frameQ);
    }

    void ChangedInflection()
    {
        SetQuaternion();
    }

    void LoadMeshTo(std::shared_ptr<Device> device);
    void UpdateMeshTo(std::shared_ptr<Device> device);
    void DrawModelOn(std::shared_ptr<Device> device);
    void DrawDiagonalOn(std::shared_ptr<Device> device);

    glm::mat3 inertia_tensor, inv_inertia_tensor; // wzgledem poczatku ukladu wspolrzednych w bryle z osia Y na glownej przekanej
    glm::quat Q = glm::quat(1, 0, 0, 0); // (cos(0), 0,0,0) // brak obrotu 
    glm::quat frameQ;
    const float hp = glm::half_pi<float>();
    float velocity = hp; // pi/2 rad na sek
    glm::vec3 mass_center;
    float density = 1.0f;
    float mass;

    float color[4] = { 0.8f, 0.2f, 0.5f, 0.5f };
    bool visible = true;
    float dt = 0.01f;
    float maxSize = 1.0f;
    float inflection = 0.0f;
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


