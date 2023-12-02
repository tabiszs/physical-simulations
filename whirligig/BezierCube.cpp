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

    // force on corners
    //for (int i = 0; i < corner_count; ++i)
    //{
    //    auto& x1 = cc_corners[i];
    //    auto& x = positions[jelly_corners_idx[i]];   
    //    auto& xt = velocities[jelly_corners_idx[i]];

    //    const auto [dx, dxt] = RungeKutta4(x1, x, xt);

    //    velocities[jelly_corners_idx[i]] += dxt;
    //    positions[jelly_corners_idx[i]] += dx;
    //}

    // force inside    

    //// precomputing bounce component
    //auto computed_spring_bounce = std::array<glm::vec3, 6 * 64>{};
    //auto is_computed_spring = std::array<bool, 6 * 64>{};
    //for (int i = 0; i < tree_of_congruence_vertices.size(); ++i)
    //{
    //    int j = 0;
    //    for (auto it = tree_of_congruence_vertices[i].begin(); it != tree_of_congruence_vertices[i].end(); ++it,++j)
    //    {
    //        const auto& p = (*it).first;
    //        int idx = i * 6 + j; // decode index. 
    //        if (!is_computed_spring[idx])
    //        {
    //            const auto& is_on_edge = (*it).second;
    //            auto distance_diff = positions[p] - positions[i];
    //            auto distance_norm = std::sqrt(glm::dot(distance_diff, distance_diff));
    //            auto l = is_on_edge ? one_div_three : sqrt_two_div_three;
    //            auto bounce = distance_norm == 0 ? glm::vec3() : (glm::vec3(c2, c2, c2) * (distance_norm - l) / distance_norm); // * distance_diff (different from every point)
    //            computed_spring_bounce[idx] = bounce;
    //            is_computed_spring[idx] = true;
    //        }            
    //    }
    //}

    // for every point compute velocity end position
    // include springs with control cube //TODO
    for (int i = 0, j=0; i < tree_of_congruence_vertices.size(); ++i)
    {
        // oblicz sile dla kazdego wierzcholka
        //int j = 0;
        //glm::vec3 fi{};
        //for (auto it = tree_of_congruence_vertices[i].begin(); it != tree_of_congruence_vertices[i].end(); ++it, ++j)
        //{
        //    const auto& p = (*it).first;
        //    int idx = i * 6!!! (max 2*3 (edges) + 4*4 (faces) + j; // todo i-mniejsze, j-wieksze -> pozwoli na optymalizacje
        //    
        //    auto& x1 = positions[p];
        //    auto& x0 = positions[i];
        //    fi += computed_spring_bounce[idx] * (x1 - x0);
        //}
        // 
        // oblicz nowa predkosc i pozycje



        std::list<std::pair<glm::vec3, bool>> neighbours{};
        for (auto it = tree_of_congruence_vertices[i].begin(); it != tree_of_congruence_vertices[i].end(); ++it)
        {
            neighbours.push_back({positions[(*it).first], (*it).second});
        }
        auto& x = positions[i];
        auto& xt = velocities[i];

        if (j < jelly_corners_idx.size() && jelly_corners_idx[j] == i)
        {
            const auto& cc = cc_corners[j];
            const auto [dx, dxt] = RungeKutta4(cc, neighbours, x, xt);
            velocities[i] += dxt;
            positions[i] += dx;
            j++;
        }
        else
        {
            const auto [dx, dxt] = RungeKutta4(neighbours, x, xt);
            velocities[i] += dxt;
            positions[i] += dx;
        }
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

void BezierCube::SetTreeOfCongruence()
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

std::tuple<glm::vec3, glm::vec3> BezierCube::RungeKutta4(const glm::vec3& cc, std::list<std::pair<glm::vec3, bool>>& neighbours, const glm::vec3& x, const glm::vec3& xt)
{
    auto dxtdt = [this](const glm::vec3& cc, std::list<std::pair<glm::vec3, bool>>& neighbours, const glm::vec3& x, const glm::vec3& xt) {
        auto distance_diff = cc - x;
        auto distance_norm = std::sqrt(glm::dot(distance_diff, distance_diff));
        auto f = distance_norm == 0 ? glm::vec3() : (glm::vec3(c2, c2, c2) * (distance_norm - 0) * distance_diff / distance_norm);
        for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
        {
            const auto& is_on_edge = (*it).second;
            auto l = is_on_edge ? one_div_three : sqrt_two_div_three;
            auto distance_diff = (*it).first - x;
            auto distance_norm = std::sqrt(glm::dot(distance_diff, distance_diff));
            f += distance_norm == 0 ? glm::vec3() : (glm::vec3(c1, c1, c1) * (distance_norm - l) * distance_diff / distance_norm);
        }
        auto g = -glm::vec3(k, k, k) * xt;
        auto F = f + g;
        return F / mass;
    };

    auto dxdt = [this](const glm::vec3& xt) {
        return xt;
    };
    
    glm::vec3 k0_xt = dt * dxtdt(cc, neighbours, x, xt);
    glm::vec3 k0_x = dt * dxdt(xt);

    glm::vec3 k1_xt = dt * dxtdt(cc, neighbours, x + k0_x / 2.0f, xt + k0_xt / 2.0f);
    glm::vec3 k1_x = dt * dxdt(xt + k0_xt / 2.0f);

    glm::vec3 k2_xt = dt * dxtdt(cc, neighbours, x + k1_xt / 2.0f, xt + k1_xt / 2.0f);
    glm::vec3 k2_x = dt * dxdt(xt + k1_xt / 2.0f);

    glm::vec3 k3_xt = dt * dxtdt(cc, neighbours, x + k2_xt, xt + k2_xt);
    glm::vec3 k3_x = dt * dxdt(xt + k2_xt);

    auto dxt = (k0_xt + 2.0f * k1_xt + 2.0f * k2_xt + k3_xt) / 6.0f;
    auto dx = (k0_x + 2.0f * k1_x + 2.0f * k2_x + k3_x) / 6.0f;

    return std::tuple<glm::vec3, glm::vec3>(dx, dxt);
}

std::tuple<glm::vec3, glm::vec3> BezierCube::RungeKutta4(std::list<std::pair<glm::vec3, bool>>& neighbours, const glm::vec3& x, const glm::vec3& xt)
{
    auto dxtdt = [this](const std::list<std::pair<glm::vec3, bool>>& neighbours, const glm::vec3& x, const glm::vec3& xt) {
        glm::vec3 f{};
        for (auto it=neighbours.begin(); it != neighbours.end(); ++it)
        {
            const auto& is_on_edge = (*it).second;
            auto l = is_on_edge ? one_div_three : sqrt_two_div_three;
            auto distance_diff = (*it).first - x;
            auto distance_norm = std::sqrt(glm::dot(distance_diff, distance_diff));
            f += distance_norm == 0 ? glm::vec3() : (glm::vec3(c1, c1, c1) * (distance_norm - l) * distance_diff / distance_norm);
        }        
        auto g = -glm::vec3(k, k, k) * xt;
        auto F = f + g;
        return F / mass;
    };

    auto dxdt = [this](const glm::vec3& xt) {
        return xt;
    };

    glm::vec3 k0_xt = dt * dxtdt(neighbours, x, xt);
    glm::vec3 k0_x = dt * dxdt(xt);

    glm::vec3 k1_xt = dt * dxtdt(neighbours, x + k0_x / 2.0f, xt + k0_xt / 2.0f);
    glm::vec3 k1_x = dt * dxdt(xt + k0_xt / 2.0f);

    glm::vec3 k2_xt = dt * dxtdt(neighbours, x + k1_xt / 2.0f, xt + k1_xt / 2.0f);
    glm::vec3 k2_x = dt * dxdt(xt + k1_xt / 2.0f);

    glm::vec3 k3_xt = dt * dxtdt(neighbours, x + k2_xt, xt + k2_xt);
    glm::vec3 k3_x = dt * dxdt(xt + k2_xt);

    auto dxt = (k0_xt + 2.0f * k1_xt + 2.0f * k2_xt + k3_xt) / 6.0f;
    auto dx = (k0_x + 2.0f * k1_x + 2.0f * k2_x + k3_x) / 6.0f;

    return std::tuple<glm::vec3, glm::vec3>(dx, dxt);
}
