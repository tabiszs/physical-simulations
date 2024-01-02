#pragma once
#include "Object.h"
#include "device.h"
#include "Cylinder.h"
#include "PumaEffector.h"

class Puma
{
public: 
	Puma(float radius = 0.5f, float height = 1.0f, int n = 20)
	{
		joint1 = std::make_shared<CenterCylinder>(red, glm::vec3(0,0,0), 0.2f, 0.4f);
		arm1 = std::make_shared<Cylinder>(yellow, glm::vec3(0, 0, 0), 0.15f, l1);
		joint2 = std::make_shared<CenterCylinder>(red, glm::vec3(0, 0, 0), 0.2f, 0.4f);
		joint2->ObjectRotation({ 0.0f, 0.0f, glm::half_pi<float>() });
		arm2 = std::make_shared<Cylinder>(green, glm::vec3(0, 0, 0), 0.15f, l2);
		joint3 = std::make_shared<CenterCylinder>(red, glm::vec3(0, 0, 0), 0.2f, 0.4f);
		joint3->ObjectRotation({ 0.0f, 0.0f, glm::half_pi<float>() });
		arm3 = std::make_shared<Cylinder>(yellow, glm::vec3(0, 0, 0), 0.15f, l3);
		joint4 = std::make_shared<CenterCylinder>(red, glm::vec3(0, 0, 0), 0.2f, 0.4f);
		arm4 = std::make_shared<Cylinder>(yellow, glm::vec3(0, 0, 0), 0.15f, l4);
		arm4->ObjectRotation({ 0.0f, 0.0f, glm::half_pi<float>() });
		joint5 = std::make_shared<CenterCylinder>(red, glm::vec3(0, 0, 0), 0.2f, 0.2f); // TODO
		effector = std::make_shared<PumaEffector>();

		arm1->SetParent(joint1);
		joint2->SetParent(arm1);
		arm2->SetParent(joint2);
		joint3->SetParent(arm2);
		arm3->SetParent(joint3);
		joint4->SetParent(arm3);
		arm4->SetParent(joint4);
		joint5->SetParent(arm4);
		effector->SetParent(joint5);
	}

	void LoadMeshTo(std::shared_ptr<Device> device);
	void Update(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);

	void SetLengthOfArm1(float l);
	void SetLengthOfArm2(float l);
	void SetLengthOfArm3(float l);
	void SetLengthOfArm4(float l);
	void SetAngleOfJoint1(float r);
	void SetAngleOfJoint2(float r);
	void SetAngleOfJoint3(float r);
	void SetAngleOfJoint4(float r);
	void SetAngleOfJoint5(float r);

	std::shared_ptr<Cylinder> arm1;
	std::shared_ptr<Cylinder> arm2;
	std::shared_ptr<Cylinder> arm3;
	std::shared_ptr<Cylinder> arm4;
	std::shared_ptr<CenterCylinder> joint1;
	std::shared_ptr<CenterCylinder> joint2;
	std::shared_ptr<CenterCylinder> joint3;
	std::shared_ptr<CenterCylinder> joint4;
	std::shared_ptr<CenterCylinder> joint5;
	std::shared_ptr<PumaEffector> effector;

private:
	float q1{}, q2{}, q3{}, q4{}, q5{};
	float l1 = 1.0f, l2 = 1.0f, l3 = 1.0f/* variable */, l4 = 1.0f;
};

