#pragma once
#include "Object.h"
#include "device.h"
#include "Chain.h"

class Cylinder : public Object, public Chain
{
public:
	Cylinder(const glm::vec4 color, glm::vec3 center_point = {}, 
		float radius = 0.5f, float height = 1.0f, int n = 20)
	{
		this->color = color;
		this->radius = radius;
		this->height = height;
		this->n = n;
		vert = std::vector<glm::vec3>(2 * n + 2);
		vertices = std::vector<GLfloat>(3 * vert.size());
		GenerateMesh(radius, 1.0f, n);
		UpdateBuffer();
		GenerateIndices();
		shader = ShaderHolder::Get().NewSimpleShader();
		center_position = center_point;
	}

	glm::mat4 ModelMatrix();
	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	void ObjectRotation(glm::vec3 euler_angles);
	void SetHeight(float height);
	void SetCenterPosition(glm::vec3 p);

	glm::vec3 GetEndPoint();

protected:
	void GenerateMesh(float radius, float height, int n);
	void GenerateIndices();
	void UpdateBuffer();
	virtual glm::mat4 ObjectTransformation();
	glm::mat4 Frame() override;

	std::vector<glm::vec3> vert{};

	glm::vec3 center_position;
	glm::quat quaternion = glm::quat(1,0,0,0);
	glm::quat object_rotation = glm::quat(1, 0, 0, 0);
	float radius, height;
	int n;
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

	glm::mat4 Frame() override;
	glm::mat4 ObjectTransformation() override;
	void SetAngle(float angle);
};