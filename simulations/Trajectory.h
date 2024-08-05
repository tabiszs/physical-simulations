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
		vertices.reserve(3 * max_trajectory);
		indices = std::vector<GLuint>(max_trajectory);
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
		if (indices_counter >= length)
		{
			pop_first();
		}
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
		device->DrawLinesStrip((Object*)this, indices_counter, 0);
	}

	void Clean()
	{
		indices_counter = 0;
		vertices.clear();
	}

	int indices_counter = 0;
	int length = 1000;
	int max_trajectory = 100000;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};

