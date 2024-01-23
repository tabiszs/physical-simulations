#pragma once
#include "Object.h"
#include "device.h"
class Circle : public Object
{
public:
	Circle(int num_segments, glm::vec3 center_pos, float radius, glm::vec4 col)
	{
		vertices.resize(3*num_segments);
		indices.resize(num_segments);
		for (int ii = 0; ii < num_segments; ii++) {
			float theta = glm::two_pi<float>() * float(ii) / float(num_segments);
			float x = radius * cosf(theta);
			float y = radius * sinf(theta);
			vertices[3 * ii + 0] = center_pos.x + x;
			vertices[3 * ii + 1] = center_pos.y + y;
			vertices[3 * ii + 2] = center_pos.z + 0;
			indices[ii] = ii;
		}

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
private:
	glm::quat quaternion{ 1,0,0,0 };
	glm::vec3 position{ 0,0,0 };
	float scale = 1.0f;
};

