#pragma once
#include <numeric>
#include "Object.h"
#include "device.h"

class Trajectory : public Object
{
public:
	Trajectory()
	{
		shader = ShaderHolder::Get().trajectoryShader;
		vertices.reserve((GLfloat)3 * max_trajectory);
		indices.reserve(max_trajectory);
		std::iota(std::begin(indices), std::end(indices), 0);
	}

	void pop_first()
	{
		vertices.erase(vertices.begin(), vertices.begin()+3);
		indices_counter--;
		need_update = true;
	}

	void push_back(glm::vec3 vertex)
	{
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
		indices_counter++;
		need_update = true;
	}

	void LoadMeshTo(std::shared_ptr<Device> device)
	{
		device->LoadMesh((Object*)this);
		shader->use();
		shader->set4Float("objectColor", color);
	}

	void UpdateMeshTo(std::shared_ptr<Device> device)
	{
		device->UpdateMesh((Object*)this);
	}

	void DrawModelOn(std::shared_ptr<Device> device)
	{
		shader->use();
		device->DrawLines((Object*)this, indices_counter, 0);
	}

	int indices_counter = 0;
	int max_trajectory = 100000;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};

