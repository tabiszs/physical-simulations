#pragma once
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float M_2PI       = 6.283185307f;
const float M_PI        = 3.14159265358979323846f;
const float M_PIDIV2    = 1.57079632679489661923f;
const float M_PIDIV4    = 0.785398163f;

typedef struct tagSIZE
{
    long cx;
    long cy;
} SIZE, POINT;

typedef struct tagRECT
{
    float left;
    float right;
    float top;
    float bottom;
} RECT;

// https://github.com/Microsoft/DirectXMath
// Modulo the range of the given angle such that -XM_PI <= Angle < XM_PI
float ScalarModAngle(float Angle);
glm::vec4 Vec3TransformCoord(glm::mat4 mat, glm::vec3 vec);