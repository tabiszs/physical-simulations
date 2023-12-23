#pragma once
#include "Object.h"
#include "device.h"

class Cylinder : public Object
{
public:
	Cylinder(const glm::vec4 color, glm::vec3 center_point = {}, 
		float radius = 0.5f, float height = 1.0f, int n = 20)
	{
		this->color = color;
		GenerateMesh(center_point, radius, height, n);
		shader = ShaderHolder::Get().NewSimpleShader();
		center_position = center_point;
	}

	glm::mat4 ModelMatrix();
	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	void Rotate(glm::vec3 euler_angles);

protected:
	void GenerateMesh(glm::vec3 center_point, float radius, float height, int n);
	void UpdateBuffer();

	std::vector<glm::vec3> vert{};

	glm::vec3 center_position;
	glm::quat quaternion{};
};

class CenterCylinder : public Cylinder
{
public:
	CenterCylinder(const glm::vec4 color, glm::vec3 center_point = {},
		float radius = 0.5f, float height = 1.0f, int n = 20)
		: Cylinder(color, center_point - glm::vec3(0.0f, height / 2.0f, 0.0f), radius, height, n) 
	{ 
		center_position = center_point;
	}

};