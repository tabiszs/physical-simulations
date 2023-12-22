#pragma once
#include <memory>
#include <vector>
#include "shader.h"
#include "ShaderHolder.h"
#include "mat.h"

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
	float grey[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

private:
	
};

