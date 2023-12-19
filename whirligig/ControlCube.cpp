#include "ControlCube.h"
#include <glm/gtx/euler_angles.hpp>

glm::mat4 ControlCube::ModelMatrix()
{
    auto t = Mat::translation(position);
    auto r = glm::eulerAngleXYZ(euler_angles[0], euler_angles[1], euler_angles[2]);
    return t * r ;
}

std::array<glm::vec3, 8> ControlCube::GetCornersPositions()
{
    auto mtx = ModelMatrix();
    std::array<glm::vec3, 8> corners{};
    corners[0] = mtx * glm::vec4(xMin, yMin, zMin, 1);
    corners[1] = mtx * glm::vec4(xMin, yMin, zMax, 1);
    corners[2] = mtx * glm::vec4(xMin, yMax, zMin, 1);
    corners[3] = mtx * glm::vec4(xMin, yMax, zMax, 1);
    corners[4] = mtx * glm::vec4(xMax, yMin, zMin, 1);
    corners[5] = mtx * glm::vec4(xMax, yMin, zMax, 1);
    corners[6] = mtx * glm::vec4(xMax, yMax, zMin, 1);
    corners[7] = mtx * glm::vec4(xMax, yMax, zMax, 1);
    return corners;
}

void ControlCube::Restart()
{
    position = { 1.0f, 0.0f, 0.0f };
    euler_angles = { 0.0f, 0.0f, 0.0f };
    need_update = true;
}
