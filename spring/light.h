#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light(glm::vec3 pos) { position = pos; }

	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 position;
};

