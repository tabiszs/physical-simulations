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
    shader->setMatrix4F("modelMtx", ModelMatrix());
    point_shader->use();
    point_shader->set4Float("objectColor", blue);
    point_shader->setMatrix4F("modelMtx", ModelMatrix());
}

void BezierCube::UpdateMeshTo(std::shared_ptr<Device> device)
{
    UpdateBuffer();
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

std::array<int, 8> BezierCube::GetCornersPositions()
{
    return { 0,3,12,15,48,51,60,63 };
}

void BezierCube::ComputeForce(const std::array<glm::vec3, 8>& cc_corners)
{
    const int corner_count = 8;
    auto jelly_corners_idx = GetCornersPositions();

    for (int i = 0; i < corner_count; ++i)
    {
        auto& x1 = cc_corners[i];
        auto& x = positions[jelly_corners_idx[i]];   
        auto& xt = velocities[jelly_corners_idx[i]];

        const auto [dx, dxt] = RungeKutta4(x1, x, xt);

        velocities[jelly_corners_idx[i]] += dxt;
        positions[jelly_corners_idx[i]] += dx;
    }
    need_update = true;
}

void BezierCube::SetVerticesAndLines()
{
    int i = 0;
    vertices.reserve(3 * n * n * n);
    for (float x = 0.0f; x <= 1.0f; x += 1.0f / (n - 1))
    {
        for (float y = 0.0f; y <= 1.0f; y += 1.0f / (n - 1))
        {
            for (float z = 0.0f; z <= 1.0f; z += 1.0f / (n - 1))
            {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                int idx = i++;
                positions[idx] = glm::vec3(x, y, z);
                velocities[idx] = glm::vec3();
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

void BezierCube::UpdateBuffer()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                int idx = (i * n + j) * n + k;
                auto& pos = positions[idx];
                vertices[3 * idx + 0] = pos.x;
                vertices[3 * idx + 1] = pos.y;
                vertices[3 * idx + 2] = pos.z;
            }
        }
    }
}

std::tuple<glm::vec3, glm::vec3> BezierCube::RungeKutta4(const glm::vec3& x1, const glm::vec3& x, const glm::vec3& xt)
{
    auto dxtdt = [this](const glm::vec3& x1, const glm::vec3& x, const glm::vec3& xt) {
        auto distance_diff = x1 - x;
        auto distance_norm = std::sqrt(glm::dot(distance_diff, distance_diff));
        auto f = distance_norm == 0 ? glm::vec3() : (glm::vec3(c2, c2, c2) * (distance_norm - 0) * distance_diff / distance_norm);
        auto g = -glm::vec3(k, k, k) * xt;
        auto F = f + g;
        return F / mass;
    };

    auto dxdt = [this](const glm::vec3& xt) {
        return xt;
    };
    
    glm::vec3 k0_xt = dt * dxtdt(x1, x, xt);
    glm::vec3 k0_x = dt * dxdt(xt);

    glm::vec3 k1_xt = dt * dxtdt(x1, x + k0_x / 2.0f, xt + k0_xt / 2.0f);
    glm::vec3 k1_x = dt * dxdt(xt + k0_xt / 2.0f);

    glm::vec3 k2_xt = dt * dxtdt(x1, x + k1_xt / 2.0f, xt + k1_xt / 2.0f);
    glm::vec3 k2_x = dt * dxdt(xt + k1_xt / 2.0f);

    glm::vec3 k3_xt = dt * dxtdt(x1, x + k2_xt, xt + k2_xt);
    glm::vec3 k3_x = dt * dxdt(xt + k2_xt);

    auto dxt = (k0_xt + 2.0f * k1_xt + 2.0f * k2_xt + k3_xt) / 6.0f;
    auto dx = (k0_x + 2.0f * k1_x + 2.0f * k2_x + k3_x) / 6.0f;

    return std::tuple<glm::vec3, glm::vec3>(dx, dxt);

        //auto& x1 = cc_corners[i];
        //auto& x2 = positions[jelly_corners_idx[i]];
        //auto distance_diff = x1 - x2;
        //auto distance_norm = std::sqrt(glm::dot(distance_diff, distance_diff));
        //auto f = distance_norm == 0 ? glm::vec3() : (glm::vec3(c2,c2,c2) * (distance_norm - 0) * distance_diff / distance_norm);
        //auto g = -glm::vec3(k, k, k) * velocities[jelly_corners_idx[i]];
        //auto F = f + g;
        //auto xtt = F / mass;
        //velocities[jelly_corners_idx[i]] += dt * xtt;
        //positions[jelly_corners_idx[i]] += dt * velocities[jelly_corners_idx[i]];
}
