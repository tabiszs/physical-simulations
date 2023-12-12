#pragma once
#include "Object.h"
#include "device.h"

class Block : public Object
{
public:
	Block(glm::vec3 initial_pos)
	{
		static int id_generator = 0;
		id = ++id_generator;
		vertices = {       
			initial_pos.x, initial_pos.y, initial_pos.z, // top right
			initial_pos.x, initial_pos.y, initial_pos.z, // bottom right
			initial_pos.x, initial_pos.y, initial_pos.z, // bottom left 
			initial_pos.x, initial_pos.y, initial_pos.z, // top left			
		};
		indices = {
			0, 1, 3,
			1, 2, 3
		};

		top_left = bottom_left = top_right = bottom_right = initial_pos;
		shader = ShaderHolder::Get().planeShader;
	}

	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	glm::mat4 ModelMatrix() { return Mat::identity(); }
	glm::vec3 GetTopLeft();
	glm::vec3 GetTopRight();
	glm::vec3 GetBottomLeft();
	glm::vec3 GetBottomRight();
	void UpdateBlockSize(const glm::vec3& right_bottom);
	bool PointIsInside(glm::vec3 pt);
	bool HasIntersection(glm::vec3 pt1, glm::vec3 pt2);

	void SetInitMovingPoint(glm::vec3 p);
	void Move(glm::vec3 p);

	glm::vec3 prev_move_pos{};

	glm::vec2 top_left;
	glm::vec2 top_right;
	glm::vec2 bottom_left;
	glm::vec2 bottom_right;
	float color[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
	bool visible = true;
	int id;
};

