#include "Cursor.h"

glm::mat4 Cursor::ModelMatrix()
{
    float s = 1.0f / (maxPt - minPt);
    return Mat::scale(s,s,s);
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

void Cursor::Update(float t)
{
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
