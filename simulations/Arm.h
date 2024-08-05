#pragma once
#include "Object.h"
#include "device.h"

class Arm : public Object
{
public:
	Arm(float angle, float length, std::shared_ptr<Arm> parent = nullptr)
	{
		this->angle = angle;
		this->length = length;
		this->parent = parent;
		anchor_point = parent == nullptr ? glm::vec3() : parent->GetEffectorPoint();
		effector_point = GetEffectorPoint();
		const auto& end_point = GetEffectorPoint();
		vertices = {      
			anchor_point.x, anchor_point.y, anchor_point.z,				 // start arm position
			effector_point.x, effector_point.y, effector_point.z		 // end point position	
		};
		indices = {0,1};
		shader = ShaderHolder::Get().armShader;
	}

	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	glm::vec3 GetEffectorPoint();
	glm::vec3 GetAnchorPoint();
	float GetSumAngle();
	void SetChild(std::shared_ptr<Arm> child);
	void SetEffectorPos(glm::vec3 pos);
	void SetAnchorPos(glm::vec3 pos);
	void SetLength(float l);
	void SetAngle(float a);
	void UpdateAngle();
	void UpdateEfectorPos();


	std::shared_ptr<Arm> parent = nullptr;
	std::shared_ptr<Arm> child = nullptr;
	glm::vec3 anchor_point{};
	glm::vec3 effector_point{};
	float length = 0.0f;
	float angle = 0.0f;
private:
	void Update();
	void UpdateChild();
	void UpdateChain();

	float color[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

