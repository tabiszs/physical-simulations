#include "Cursor.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Quaternion.h"

glm::mat4 Cursor::ModelMatrix() const
{
    float s = 1.0f / (maxPt - minPt);
    auto scale = Mat::scale(s, s, s);
    auto t = Mat::translation({ position[0],position[1] ,position[2] });
    auto r = glm::toMat4(quaternion);
    return t * r * scale;
}

void Cursor::LoadMeshTo(std::shared_ptr<Device> device)
{
    device->LoadPositionsAndColor((Object*)this);
}

void Cursor::UpdateMeshTo(std::shared_ptr<Device> device)
{
    device->UpdateMesh((Object*)this);
}

void Cursor::DrawModelOn(std::shared_ptr<Device> device)
{
    shader->use();
    device->DrawTriangles((Object*)this);
}

void Cursor::SetPosition(const glm::vec3& position)
{
    this->position[0] = position[0];
    this->position[1] = position[1];
    this->position[2] = position[2];
    need_update = true;
}

void Cursor::SetRotation(const glm::vec3& euler_angles)
{
    this->euler_angles = euler_angles;
    need_update = true;
}

void Cursor::SetRotation(const glm::quat& quaternion)
{
    this->quaternion = quaternion;
    need_update = true;
}

void Cursor::ImproveShortestPath(const glm::vec3& euler_angles)
{
    float d0 = this->euler_angles[0] - euler_angles[0];
    float d1 = this->euler_angles[1] - euler_angles[1];
    float d2 = this->euler_angles[2] - euler_angles[2];
    if (abs(d0) > glm::pi<float>())
    {
        if (d0 > 0) this->euler_angles[0] -= glm::two_pi<float>();
        if (d0 < 0) this->euler_angles[0] += glm::two_pi<float>();
    }
    if (abs(d1) > glm::pi<float>())
    {
        if (d1 > 0) this->euler_angles[1] -= glm::two_pi<float>();
        if (d1 < 0) this->euler_angles[1] += glm::two_pi<float>();
    }
    if (abs(d2) > glm::pi<float>())
    {
        if (d2 > 0) this->euler_angles[2] -= glm::two_pi<float>();
        if (d2 < 0) this->euler_angles[2] += glm::two_pi<float>();
    }
    SetEulerAngles(this->euler_angles);
}

void Cursor::SetEulerAngles(const glm::vec3& euler_angles)
{
    this->euler_angles = euler_angles;
    quaternion = glm::quat(euler_angles);
    need_update = true;
}

void Cursor::SetQuaternion(const glm::quat& quaternion)
{
    this->quaternion = glm::normalize(quaternion);
    euler_angles = glm::eulerAngles(this->quaternion);
    need_update = true;
}

void Cursor::AddAxis(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, glm::vec3 color)
{
    std::vector<GLuint> axis_indices = {
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
    };
    for (int i = 0; i < axis_indices.size(); ++i)
    {
        axis_indices[i] += vertices.size()/6;
    }
    indices.insert(indices.end(), axis_indices.begin(), axis_indices.end());

    vertices.insert(
        vertices.end(), {
        // positions      // color
        xMin, yMin, zMax, color.r, color.g, color.b,
        xMax, yMin, zMax, color.r, color.g, color.b,
        xMin, yMax, zMax, color.r, color.g, color.b,
        xMax, yMax, zMax, color.r, color.g, color.b,
        xMin, yMin, zMin, color.r, color.g, color.b,
        xMax, yMin, zMin, color.r, color.g, color.b,
        xMin, yMax, zMin, color.r, color.g, color.b,
        xMax, yMax, zMin, color.r, color.g, color.b
        });
}
