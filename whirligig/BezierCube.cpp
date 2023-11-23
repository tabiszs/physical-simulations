#include "BezierCube.h"

glm::mat4 BezierCube::ModelMatrix()
{
    return Mat::identity();
}

void BezierCube::LoadMeshTo(std::shared_ptr<Device> device)
{
    device->LoadMesh((Object*)this);
    shader->use();
    shader->set4Float("objectColor", yellow);
    point_shader->use();
    point_shader->set4Float("objectColor", blue);
}

void BezierCube::UpdateMeshTo(std::shared_ptr<Device> device)
{
    device->UpdateMesh((Object*)this);
}

void BezierCube::DrawModelOn(std::shared_ptr<Device> device)
{
    shader->use();
    device->DrawPatches16((Object*)this, 16*6, 288, false); // only on edge surfaces
}

void BezierCube::DrawPointsOn(std::shared_ptr<Device> device)
{
    point_shader->use();
    device->DrawPoints((Object*)this);
}

void BezierCube::DrawEdgesOn(std::shared_ptr<Device> device)
{
    point_shader->use();
    device->DrawLines((Object*)this);
}

void BezierCube::Update(float t)
{
}

void BezierCube::SetVerticlesAndLines()
{
    const int n = 4;
    vertices.reserve(n * n * n);
    for (float x = 0.0f; x <= 1.0f; x += 1.0f / (n - 1))
    {
        for (float y = 0.0f; y <= 1.0f; y += 1.0f / (n - 1))
        {
            for (float z = 0.0f; z <= 1.0f; z += 1.0f / (n - 1))
            {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
    }

    vertices[15] -= 1.0f;

    // z-axis
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n - 1; ++k)
            {
                int idx = (i * n + j) * n + k;
                indices.push_back(idx);
                indices.push_back(idx + 1);
            }
        }
    }

    // y-axis
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n - 1; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                int idx = (i * n + j) * n + k;
                indices.push_back(idx);
                indices.push_back(idx + n);
            }
        }
    }

    // x-axis
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                int idx = (i * n + j) * n + k;
                indices.push_back(idx);
                indices.push_back(idx + n * n);
            }
        }
    }

    // bezier patches    
    for (int i = 0, j = 0; j < n; ++j)
    {
        for (int k = 0; k < n; ++k)
        {
            indices.push_back((i * n + j) * n + k); // -x face
        }
    }
    for (int i = 3, j = 0; j < n; ++j)
    {
        for (int k = 0; k < n; ++k)
        {
            indices.push_back((i * n + j) * n + k); // y face
        }
    }
    
    for (int j = 0, i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            indices.push_back((i * n + j) * n + k); // -y face
        }
    }
    for (int j = 3, i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            indices.push_back((i * n + j) * n + k); // y face
        }
    }

    for (int k = 0, i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            indices.push_back((i * n + j) * n + k); // -z face
        }
    }
    for (int k = 3, i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            indices.push_back((i * n + j) * n + k); // z face
        }
    }
}
