#pragma once
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mat
{
public:
	static glm::mat4 identity();
	static glm::mat4 translation(glm::vec3 vec);
	static glm::mat4 translation(glm::vec4 vec);
	static glm::mat4 rotationX(float alpha);
	static glm::mat4 rotationY(float alpha);
	static glm::mat4 rotationZ(float alpha);
	static glm::mat4 scale(float sx, float sy, float sz);
	static glm::mat4 scale(glm::vec3 scale);
	static glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar);
	static glm::mat4 shiftedPerspective(float n, float f, float r, float l, float t, float b);
	static glm::mat4 powerToBernsteinBaseChangeMtx();
};

