#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>

struct Quaternion 
{
public:
    glm::vec4 q;

    static glm::vec3 ToEulerAngles(glm::vec4 q);
    static glm::vec4 ToQuaternion(glm::vec3 euler_angles);
    static glm::vec4 SLERP(const glm::vec4& q1, const glm::vec4& q2, float a);
    static glm::vec4 LERP(const glm::vec4& q1, const glm::vec4& q2, float a);
    static glm::vec4 Normalize(const glm::vec4& q);
    static glm::mat4 toMat4(const glm::vec4& q);
};



