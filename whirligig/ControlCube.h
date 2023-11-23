#pragma once
#include "Cube.h"

class ControlCube : public Cube
{
public:
	ControlCube()
	{
		shader = ShaderHolder::Get().frameShader;
	}
	glm::mat4 ModelMatrix() override;

	float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float euler_angles[3] = {0.0f, 0.0f, 0.0f};

};

