#include "Cylinder.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 Cylinder::ModelMatrix()
{
	const auto& preprocessing_transformation = ObjectTransformation() * Mat::scale(1, height, 1);
	glm::mat4 transformation = glm::toMat4(quaternion);
	if (parent != nullptr)
	{
		transformation = parent->Frame() * transformation;
	}
	return transformation * preprocessing_transformation;
}

glm::mat4 Cylinder::ObjectTransformation()
{
	return glm::toMat4(object_rotation);
}

glm::mat4 Cylinder::Frame()
{
	auto mtx = Mat::translation({ 0, height ,0 });
	if (parent != nullptr)
	{
		mtx = parent->Frame() * ObjectTransformation() * mtx;
	}
	return mtx;
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
	shader->use();
	shader->setMatrix4F("modelMtx", ModelMatrix());
	need_update = false;
}

void Cylinder::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTriangles((Object*)this);
}

void Cylinder::ObjectRotation(glm::vec3 euler_angles)
{
	object_rotation = glm::quat(euler_angles);
	need_update = true;
}

void Cylinder::SetHeight(float height)
{
	this->height = height;
	need_update = true;
	UpdateChildren();
}

void Cylinder::SetCenterPosition(glm::vec3 p)
{
	center_position = p;
	need_update = true;
}

glm::vec3 Cylinder::GetEndPoint()
{
	const glm::vec3 MB0 = glm::toMat4(quaternion) * glm::vec4(0, height, 0, 1);
	return center_position + MB0;
}

void Cylinder::GenerateMesh(float radius, float height, int n)
{
	float delta = glm::two_pi<float>() / (n - 1);
	float alpha = 0;
	vert[0] = { 0,0,0 };
	for (int i = 0; i < n; ++i)
	{
		vert[i+1] = glm::vec3(radius * cosf(alpha), 0.0f, radius * sinf(alpha));
		alpha += delta;
	}

	for (int i = 0; i < n; ++i)
	{
		vert[n + i + 1] = glm::vec3(radius * cosf(alpha), height, radius * sinf(alpha));
		alpha += delta;
	}
	vert[2 * n + 1] = glm::vec3(0.0f, height, 0.0f);
}

void Cylinder::GenerateIndices()
{
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

glm::mat4 CenterCylinder::Frame()
{
	auto mtx = glm::toMat4(quaternion);
	if (parent != nullptr)
	{
		mtx = parent->Frame() * mtx;
	}
	return mtx;
}

glm::mat4 CenterCylinder::ObjectTransformation()
{
	const auto half_height = glm::vec3(0, height / 2, 0);
	return glm::toMat4(object_rotation) * Mat::translation(-half_height);
}

void CenterCylinder::SetAngle(float angle)
{
	quaternion = glm::conjugate(object_rotation) * glm::quat({ 0, angle, 0 }) * object_rotation;
	need_update = true;
	UpdateChildren();
}

