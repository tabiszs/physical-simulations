#pragma once
#include "Cube.h"

class ControlCube : public Cube
{
public:
	ControlCube() : Cube(1.0f)
	{
		shader = ShaderHolder::Get().frameShader;
	}
	glm::mat4 ModelMatrix() override;
	std::array<glm::vec3, 8> GetCornersPositions();

	float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float position[3] = { 0.0f, -1.0f, 0.0f };
	float euler_angles[3] = {0.0f, 0.0f, 0.0f};

};

