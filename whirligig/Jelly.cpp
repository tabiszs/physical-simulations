#include "Jelly.h"

glm::mat4 Jelly::ModelMatrix()
{
    return Mat::identity();
}

void Jelly::LoadMeshTo(std::shared_ptr<Device> device)
{
    device->LoadMesh((Object*)this);
    shader->use();
    shader->set4Float("objectColor", yellow);
    shader->setMatrix4F("modelMtx", ModelMatrix());
    point_shader->use();
    point_shader->set4Float("objectColor", blue);
    point_shader->setMatrix4F("modelMtx", ModelMatrix());
       
}

void Jelly::UpdateMeshTo(std::shared_ptr<Device> device)
{
    UpdateBuffer();
    device->UpdateMesh((Object*)this);
}

void Jelly::DrawModelOn(std::shared_ptr<Device> device)
{
    shader->use();
    device->DrawPatches16((Object*)this, 16*6, 288, false); // only on edge surfaces
}

void Jelly::DrawPointsOn(std::shared_ptr<Device> device)
{
    point_shader->use();
    device->DrawPoints((Object*)this);
}

void Jelly::DrawEdgesOn(std::shared_ptr<Device> device)
{
    point_shader->use();
    device->DrawLines((Object*)this);
}

void Jelly::Update()
{

}

std::array<int, 8> Jelly::GetCornersPositions()
{
    return { 0,3,12,15,48,51,60,63 };
}

void Jelly::ComputeForce(const std::array<glm::vec3, 8>& cc_corners)
{
    const int corner_count = 8;
    auto jelly_corners_idx = GetCornersPositions();

    // for every point compute velocity end position
    for (int i = 0, j=0; i < n_pow_three; ++i)
    {
        auto& x = positions[i];
        auto& xt = velocities[i];

        if (j < jelly_corners_idx.size() && jelly_corners_idx[j] == i)
        {
            const auto& cc = cc_corners[j];
            RungeKutta4(i, x, xt, cc, true);            
            j++;
        }
        else
        {
            RungeKutta4(i, x, xt, x, false);
        }
    }

    // in the end update positions and velocities
    for (int i = 0; i < n_pow_three; ++i)
    {
        velocities[i] += dxt[i];
        positions[i] += dx[i];
    }
    need_update = true;
}

void Jelly::TakeCollisionsIntoAccount(glm::vec3 boundings_dimensions)
{
    for (int i = 0; i < n_pow_three; ++i)
    {
        // rekurencyjnie kazdy kierunek do czasu az po trzech wymiarach w jednej petli nie bedzie poprawki
        auto& pos = positions[i];
        auto& vel = velocities[i];
        bool has_collision = false;
        do
        {
            if (pos.x > boundings_dimensions.x)
            {
                auto diff = pos.x - boundings_dimensions.x;
                pos.x -= 2 * diff;
                vel.x = -vel.x;
                has_collision = true;
            }
            else if (pos.x < -boundings_dimensions.x)
            {
                auto diff = -boundings_dimensions.x - pos.x;
                pos.x += 2 * diff;
                vel.x = -vel.x;
                has_collision = true;
            }
            else if (pos.y > boundings_dimensions.y)
            {
                auto diff = pos.y - boundings_dimensions.y;
                pos.y -= 2 * diff;
                vel.y = -vel.y;
                has_collision = true;
            }
            else if (pos.y < -boundings_dimensions.y)
            {
                auto diff = -boundings_dimensions.y - pos.y;
                pos.y += 2 * diff;
                vel.y = -vel.y;
                has_collision = true;
            }
            else if (pos.z > boundings_dimensions.z)
            {
                auto diff = pos.z - boundings_dimensions.z;
                pos.z -= 2 * diff;
                vel.z = -vel.z;
                has_collision = true;
            }
            else if (pos.z < -boundings_dimensions.z)
            {
                auto diff = -boundings_dimensions.z - pos.z;
                pos.z += 2 * diff;
                vel.z = -vel.z;
                has_collision = true;
            }
            else
            {
                has_collision = false;
            }

        } while (has_collision);


    }
}

void Jelly::SetVerticesAndLines()
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
            indices.push_back((i * n + j) * n + k); // x face
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

void Jelly::SetTreeOfCongruence()
{
    for (int i = 0; i < n - 1; ++i) //x
    {
        for (int j = 0; j < n - 1; ++j) // y
        {
            for (int k = 0; k < n - 1; ++k) // z
            {
                int idx = (i * n + j) * n + k;
                int a1 = idx;
                int a2 = idx + 1;
                int a3 = idx + n;
                int a4 = idx + n + 1;
                int a5 = idx + n * n;
                int a6 = idx + n * n + 1;
                int a7 = idx + n * n + n;
                int a8 = idx + n * n + n + 1;

                // true - spring is on edge, false - spring is on face
                tree_of_congruence_vertices[a1].insert({ a2, true });
                tree_of_congruence_vertices[a1].insert({ a3, true });
                tree_of_congruence_vertices[a1].insert({ a5, true });
                tree_of_congruence_vertices[a1].insert({ a4, false });
                tree_of_congruence_vertices[a1].insert({ a7, false });
                tree_of_congruence_vertices[a1].insert({ a6, false });

                tree_of_congruence_vertices[a2].insert({ a1, true });
                tree_of_congruence_vertices[a2].insert({ a4, true });
                tree_of_congruence_vertices[a2].insert({ a6, true });
                tree_of_congruence_vertices[a2].insert({ a3, false });
                tree_of_congruence_vertices[a2].insert({ a8, false });
                tree_of_congruence_vertices[a2].insert({ a5, false });

                tree_of_congruence_vertices[a3].insert({ a1, true });
                tree_of_congruence_vertices[a3].insert({ a4, true });
                tree_of_congruence_vertices[a3].insert({ a7, true });
                tree_of_congruence_vertices[a3].insert({ a2, false });
                tree_of_congruence_vertices[a3].insert({ a5, false });
                tree_of_congruence_vertices[a3].insert({ a8, false });

                tree_of_congruence_vertices[a4].insert({ a2, true });
                tree_of_congruence_vertices[a4].insert({ a3, true });
                tree_of_congruence_vertices[a4].insert({ a8, true });
                tree_of_congruence_vertices[a4].insert({ a1, false });
                tree_of_congruence_vertices[a4].insert({ a6, false });
                tree_of_congruence_vertices[a4].insert({ a7, false });

                tree_of_congruence_vertices[a5].insert({ a1, true });
                tree_of_congruence_vertices[a5].insert({ a6, true });
                tree_of_congruence_vertices[a5].insert({ a7, true });
                tree_of_congruence_vertices[a5].insert({ a3, false });
                tree_of_congruence_vertices[a5].insert({ a8, false });
                tree_of_congruence_vertices[a5].insert({ a2, false });

                tree_of_congruence_vertices[a6].insert({ a2, true });
                tree_of_congruence_vertices[a6].insert({ a5, true });
                tree_of_congruence_vertices[a6].insert({ a8, true });
                tree_of_congruence_vertices[a6].insert({ a4, false });
                tree_of_congruence_vertices[a6].insert({ a7, false });
                tree_of_congruence_vertices[a6].insert({ a1, false });

                tree_of_congruence_vertices[a7].insert({ a3, true });
                tree_of_congruence_vertices[a7].insert({ a5, true });
                tree_of_congruence_vertices[a7].insert({ a8, true });
                tree_of_congruence_vertices[a7].insert({ a1, false });
                tree_of_congruence_vertices[a7].insert({ a6, false });
                tree_of_congruence_vertices[a7].insert({ a4, false });

                tree_of_congruence_vertices[a8].insert({ a6, true });
                tree_of_congruence_vertices[a8].insert({ a7, true });
                tree_of_congruence_vertices[a8].insert({ a4, true });
                tree_of_congruence_vertices[a8].insert({ a3, false });
                tree_of_congruence_vertices[a8].insert({ a2, false });
                tree_of_congruence_vertices[a8].insert({ a5, false });
            }
        }
    }
}

void Jelly::SetCornersPositions()
{
    auto jelly_corners_idx = GetCornersPositions();
    for (int i = 0, j = 0; i < n_pow_three; ++i)
    {
        if (j < jelly_corners_idx.size() && jelly_corners_idx[j] == i)
        {
            corners_positions[i] = 1;
            j++;
        }        
    }
}

void Jelly::SetNeighbours()
{
    for (int i = 0; i < tree_of_congruence_vertices.size(); ++i)
    {
        int j = 18 * i;
        neighbours_count[i] = tree_of_congruence_vertices[i].size();
        for (auto it = tree_of_congruence_vertices[i].begin(); it != tree_of_congruence_vertices[i].end(); ++it)
        {
            neighbours_list[j] = (*it).first;
            neighbours_distance[j] = (*it).second;
            j++;
        }
    }
}

void Jelly::UpdateBuffer()
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

void Jelly::RungeKutta4(int current_idx, const glm::vec3& x, const glm::vec3& xt, const glm::vec3& cc, bool corner)
{
    auto dxtdt = [this](const glm::vec3& cc, int current_idx, const glm::vec3& x, const glm::vec3& xt, bool corner) {
        glm::vec3 f = glm::vec3(0,0,0);
        if (corner)
        {
            auto distance_diff = cc - x;
            f = (glm::vec3(c2, c2, c2) *  distance_diff);
        }

        for (int i=0; i<neighbours_count[current_idx]; ++i)
        {
            const auto& is_on_edge = neighbours_distance[18 * current_idx + i];
            auto l = is_on_edge ? one_div_three : sqrt_two_div_three;
            auto distance_diff = positions[neighbours_list[18 * current_idx + i]] - x;
            auto distance_norm = std::sqrtf(glm::dot(distance_diff, distance_diff));
            auto distance_norm_l = (distance_norm - l);
            f += abs(distance_norm_l) < FLT_EPSILON ? glm::vec3() : (glm::vec3(c1, c1, c1) * distance_norm_l * distance_diff / distance_norm);
        }
        auto g = -glm::vec3(k, k, k) * xt;
        auto F = f + g;
        return F / mass;
    };

    auto dxdt = [this](const glm::vec3& xt) {
        return xt;
    };

    glm::vec3 k0_xt = dt * dxtdt(cc, current_idx, x, xt, corner);
    glm::vec3 k0_x = dt * dxdt(xt);

    glm::vec3 k1_xt = dt * dxtdt(cc, current_idx, x + k0_x / 2.0f, xt + k0_xt / 2.0f, corner);
    glm::vec3 k1_x = dt * dxdt(xt + k0_xt / 2.0f);

    glm::vec3 k2_xt = dt * dxtdt(cc, current_idx, x + k1_xt / 2.0f, xt + k1_xt / 2.0f, corner);
    glm::vec3 k2_x = dt * dxdt(xt + k1_xt / 2.0f);

    glm::vec3 k3_xt = dt * dxtdt(cc, current_idx, x + k2_xt, xt + k2_xt, corner);
    glm::vec3 k3_x = dt * dxdt(xt + k2_xt);

    dxt[current_idx] = (k0_xt + 2.0f * k1_xt + 2.0f * k2_xt + k3_xt) / 6.0f;
    dx[current_idx] = (k0_x + 2.0f * k1_x + 2.0f * k2_x + k3_x) / 6.0f;
}

void Jelly::CheckCollisions()
{
}
