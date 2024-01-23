#pragma once
#include "Object.h"
#include "device.h"
class Line : public Object
{
public:
	Line(float start_pos, float end_pos, glm::vec4 col)
	{
		vertices.resize(6);
		indices.resize(2);

		vertices[0] = start_pos;
		vertices[1] = 0.0f;
		vertices[2] = 0.0f;

		vertices[3] = end_pos;
		vertices[4] = 0.0f;
		vertices[5] = 0.0f;

		indices[0] = 0;
		indices[1] = 1;

		color = col;
		shader = ShaderHolder::Get().NewSimpleShader();
	}

	glm::mat4 ModelMatrix() override;
	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	void Rotate(glm::vec3 euler_angles);
	void Translate(glm::vec3 t);
	void Scale(float s);
	void SetEndPositions(glm::vec3 p0, glm::vec3 p1);
private:
	glm::quat quaternion{1,0,0,0};
	glm::vec3 position{0,0,0};
	float scale = 1.0f;

};

