#include "BezierCube.h"

glm::mat4 BezierCube::ModelMatrix()
{
    return Mat::identity();
}

void BezierCube::LoadMeshTo(std::shared_ptr<Device> device)
{
    device->LoadPositionsAndNormals((Object*)this);
    shader->use();
    shader->set4Float("objectColor", yellow);
    shader->setMatrix4F("modelMtx", ModelMatrix());
}

void BezierCube::UpdateMeshTo(std::shared_ptr<Device> device)
{
    device->UpdateMesh((Object*)this);
}

void BezierCube::DrawModelOn(std::shared_ptr<Device> device)
{
    shader->use();
    device->DrawPatches16((Object*)this, 16 * 6, 0, false);
}

void BezierCube::FillBuffer(const std::vector<GLfloat>& vertices)
{
    // bezier patches
    int idx;
    for (int i = 0, j = 0; j < n; ++j)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = (i * n + j) * n + k; // -x face

            // position
            this->vertices.push_back(vertices[3 * idx]);
            this->vertices.push_back(vertices[3 * idx + 1]);
            this->vertices.push_back(vertices[3 * idx + 2]);

            // normals
            this->vertices.push_back(-1.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(0.0f);
        }
    }
    for (int i = 3, j = 0; j < n; ++j)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = (i * n + j) * n + k; // x face

            // position
            this->vertices.push_back(vertices[3 * idx]);
            this->vertices.push_back(vertices[3 * idx + 1]);
            this->vertices.push_back(vertices[3 * idx + 2]);

            // normals
            this->vertices.push_back(1.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(0.0f);
        }
    }

    for (int j = 0, i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = (i * n + j) * n + k; // -y face

            // position
            this->vertices.push_back(vertices[3 * idx]);
            this->vertices.push_back(vertices[3 * idx + 1]);
            this->vertices.push_back(vertices[3 * idx + 2]);

            // normals
            this->vertices.push_back(0.0f);
            this->vertices.push_back(-1.0f);
            this->vertices.push_back(0.0f);
        }
    }
    for (int j = 3, i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = ((i * n + j) * n + k); // y face

            // position
            this->vertices.push_back(vertices[3 * idx]);
            this->vertices.push_back(vertices[3 * idx + 1]);
            this->vertices.push_back(vertices[3 * idx + 2]);

            // normals
            this->vertices.push_back(0.0f);
            this->vertices.push_back(1.0f);
            this->vertices.push_back(0.0f);
        }
    }

    for (int k = 0, i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            idx = ((i * n + j) * n + k); // -z face

            // position
            this->vertices.push_back(vertices[3 * idx]);
            this->vertices.push_back(vertices[3 * idx + 1]);
            this->vertices.push_back(vertices[3 * idx + 2]);

            // normals
            this->vertices.push_back(0.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(-1.0f);
        }
    }
    for (int k = 3, i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            idx = ((i * n + j) * n + k); // z face

            // position
            this->vertices.push_back(vertices[3 * idx]);
            this->vertices.push_back(vertices[3 * idx + 1]);
            this->vertices.push_back(vertices[3 * idx + 2]);

            // normals
            this->vertices.push_back(0.0f);
            this->vertices.push_back(0.0f);
            this->vertices.push_back(1.0f);
        }
    }
}

void BezierCube::UpdateBuffer(const std::vector<GLfloat>& vertices)
{
    // bezier patches
    int idx, c = 0;
    for (int i = 0, j = 0; j < n; ++j)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = (i * n + j) * n + k; // -x face

            // position
            this->vertices[c++] = vertices[3 * idx];
            this->vertices[c++] = vertices[3 * idx + 1];
            this->vertices[c++] = vertices[3 * idx + 2];

            // normals
            this->vertices[c++] = 1.0f;
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = 0.0f;
        }
    }
    for (int i = 3, j = 0; j < n; ++j)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = (i * n + j) * n + k; // x face

            // position
            this->vertices[c++] = vertices[3 * idx];
            this->vertices[c++] = vertices[3 * idx + 1];
            this->vertices[c++] = vertices[3 * idx + 2];

            // normals
            this->vertices[c++] = -1.0f;
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = 0.0f;
        }
    }

    for (int j = 0, i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = (i * n + j) * n + k; // -y face

            // position
            this->vertices[c++] = vertices[3 * idx];
            this->vertices[c++] = vertices[3 * idx + 1];
            this->vertices[c++] = vertices[3 * idx + 2];

            // normals
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = -1.0f;
            this->vertices[c++] = 0.0f;
        }
    }
    for (int j = 3, i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            idx = ((i * n + j) * n + k); // y face

            // position
            this->vertices[c++] = vertices[3 * idx];
            this->vertices[c++] = vertices[3 * idx + 1];
            this->vertices[c++] = vertices[3 * idx + 2];

            // normals
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = 1.0f;
            this->vertices[c++] = 0.0f;
        }
    }

    for (int k = 0, i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            idx = ((i * n + j) * n + k); // -z face

            // position
            this->vertices[c++] = vertices[3 * idx];
            this->vertices[c++] = vertices[3 * idx + 1];
            this->vertices[c++] = vertices[3 * idx + 2];

            // normals
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = 1.0f;
        }
    }
    for (int k = 3, i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            idx = ((i * n + j) * n + k); // z face

            // position
            this->vertices[c++] = vertices[3 * idx];
            this->vertices[c++] = vertices[3 * idx + 1];
            this->vertices[c++] = vertices[3 * idx + 2];

            // normals
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = 0.0f;
            this->vertices[c++] = -1.0f;
        }
    }
}
