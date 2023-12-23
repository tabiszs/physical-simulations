#include "Cylinder.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 Cylinder::ModelMatrix()
{
	return Mat::identity();
}

void Cylinder::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", (float*) &color);
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void Cylinder::UpdateMeshTo(std::shared_ptr<Device> device)
{
	UpdateBuffer();
	device->UpdateMesh((Object*)this);
	need_update = false;
}

void Cylinder::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTriangles((Object*)this);
}

void Cylinder::Rotate(glm::vec3 euler_angles)
{
	const auto quat = glm::quat(euler_angles);
	quaternion = quat * quaternion * glm::conjugate(quat);

	for (auto& v : vert)
	{
		v = Mat::translation(center_position) * glm::toMat4(quat) * Mat::translation(-center_position) * glm::vec4(v, 1.0f);
	}
	need_update = true;
}

void Cylinder::GenerateMesh(glm::vec3 center_point, float radius, float height, int n)
{
	float delta = glm::two_pi<float>() / (n - 1);
	float alpha = 0;
	vert.emplace_back(center_point);
	for (int i = 0; i < n; ++i)
	{
		const auto v = center_point + glm::vec3(radius* cosf(alpha), 0.0f, radius* sinf(alpha));
		vert.emplace_back(v);
		alpha += delta;
	}

	for (int i = 0; i < n; ++i)
	{
		const auto v = center_point + glm::vec3(radius * cosf(alpha), height, radius * sinf(alpha));
		vert.emplace_back(v);
		alpha += delta;
	}
	const auto v = center_point + glm::vec3(0.0f, height, 0.0f);
	vert.emplace_back(v);

	for (const auto& v : vert)
	{
		vertices.emplace_back(v.x);
		vertices.emplace_back(v.y);
		vertices.emplace_back(v.z);
	}

	for (int i = 0; i < n; ++i)
	{
		indices.emplace_back(i + 1);
		indices.emplace_back(n + i + 1);
		indices.emplace_back((i + 1) % n + 1);

		indices.emplace_back(n + i + 1);
		indices.emplace_back(n + (i + 1) % n + 1);
		indices.emplace_back((i + 1) % n + 1);

		indices.emplace_back(i + 1);
		indices.emplace_back((i + 1) % n + 1);
		indices.emplace_back(0);

		indices.emplace_back(n + i + 1);
		indices.emplace_back(2 * n + 1);
		indices.emplace_back(n + (i + 1) % n + 1);
	}
}

void Cylinder::UpdateBuffer()
{
	for(int i=0; i<vert.size(); ++i)
	{
		vertices[3 * i + 0] = vert[i].x;
		vertices[3 * i + 1] = vert[i].y;
		vertices[3 * i + 2] = vert[i].z;
	}
}
