#include "Cube.h"

glm::mat4 Cube::ModelMatrix()
{
    auto v = glm::vec3(xMin, yMin, zMin);
    auto t = Mat::translation(v);
    return Mat::identity();
}

void Cube::LoadMeshTo(std::shared_ptr<Device> device)
{
    device->LoadMesh((Object*)this);
    shader->use();
    shader->set4Float("objectColor", color);
}

void Cube::UpdateMeshTo(std::shared_ptr<Device> device)
{
    device->UpdateMesh((Object*)this);
}

void Cube::DrawModelOn(std::shared_ptr<Device> device)
{
    shader->use();
    device->DrawLines((Object*)this);
}

void Cube::SetBoundings(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
    vertices.clear();
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

    this->xMin = xMin;
    this->yMin = yMin;
    this->zMin = zMin;
    this->xMax = xMax;
    this->yMax = yMax;
    this->zMax = zMax;
}
