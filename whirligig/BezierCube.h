#pragma once
#include "Object.h"
#include "device.h"
class BezierCube : public Object
{
public:
	BezierCube(const std::vector<GLfloat>& vertices)
	{
		FillBuffer(vertices);
		shader = ShaderHolder::Get().bezierCubeShader;
		for (int i = 0; i < size; ++i) indices.push_back(i);
	}

	glm::mat4 ModelMatrix() override;
	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	void FillBuffer(const std::vector<GLfloat>& vertices);
	void UpdateBuffer(const std::vector<GLfloat>& vertices);

	static const int n = 4;
	static const int size = 16 * 6;
};

