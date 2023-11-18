#pragma once
#include <memory>
#include <vector>
#include "shader.h"
#include "ShaderHolder.h"
#include "mat.h"

class Object
{
public:
	std::shared_ptr<Shader> shader;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, IBO;
	bool need_update = true;

private:
	
};

