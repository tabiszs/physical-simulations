#pragma once
#include "Object.h"
#include "device.h"

class Plane : public Object
{
public:
	Plane(std::shared_ptr<Shader> shader = ShaderHolder::Get().planeShader)
	{
		vertices = {
			// positions          
			 1.0f, 0.0f, -1.0f, // top right
			 1.0f, 0.0f,  1.0f, // bottom right
			-1.0f, 0.0f,  1.0f, // bottom left 
			-1.0f, 0.0f, -1.0f, // top left		
		};

		indices = {
			0, 1, 3,
			1, 2, 3
		};

		this->shader = shader;
	}

	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);

	glm::mat4 ModelMatrix() { return Mat::scale({ 3,3,3 })*Mat::identity(); }

	float color[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	bool visible = true;
};

