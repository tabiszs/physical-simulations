#pragma once
#include <memory>
#include <vector>
#include "shader.h"
#include "ShaderHolder.h"
#include "mat.h"

const glm::vec4 grey = { 0.5f, 0.5f, 0.5f, 1.0f };
const glm::vec4 blue = { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 green = { 0.0f, 1.0f, 0.0f, 1.0f };

class Object
{
public:
	virtual glm::mat4 ModelMatrix();

	std::shared_ptr<Shader> shader;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, IBO;
	bool need_update = true;

protected:
	glm::vec4 color;
};

