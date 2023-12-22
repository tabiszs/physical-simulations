#include "Arm.h"

void Arm::LoadMeshTo(std::shared_ptr<Device> device)
{
	Update();
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", color);
}

void Arm::UpdateMeshTo(std::shared_ptr<Device> device)
{
	Update();
	device->UpdateMesh((Object*)this);
}

void Arm::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawLines((Object*)this);
}

glm::vec3 Arm::GetEffectorPoint()
{
	const auto& sum_angle = GetSumAngle();
	return Mat::rotationZ(sum_angle) * glm::vec4(length, 0, 0, 1) + glm::vec4(anchor_point,1);
}

glm::vec3 Arm::GetAnchorPoint()
{
	return parent == nullptr ? glm::vec3() : parent->GetEffectorPoint();
}

float Arm::GetSumAngle()
{
	return parent == nullptr ? angle : angle + parent->GetSumAngle();
}

void Arm::SetChild(std::shared_ptr<Arm> child)
{
	this->child = child;
}

void Arm::SetEffectorPos(glm::vec3 effector_point)
{
	this->effector_point = effector_point;
	need_update = true;
}

void Arm::SetAnchorPos(glm::vec3 anchor_point)
{
	this->anchor_point = anchor_point;
	need_update = true;
}

void Arm::SetLength(float l)
{
	length = l;
	const auto& effector_point = GetEffectorPoint();
	SetEffectorPos(effector_point);
	UpdateChain();
}

void Arm::SetAngle(float a)
{
	angle = a;
	const auto& end = GetEffectorPoint();
	SetEffectorPos(end);
	UpdateChain();
}

void Arm::UpdateAngle()
{
	// zalozenie ze poprzednie katy w lancuchu sa aktualne
	const auto& parent_sum_angle = parent == nullptr ? 0 : parent->GetSumAngle();
	angle = atan2f(effector_point.y - anchor_point.y, effector_point.x - anchor_point.x) - parent_sum_angle;
}

void Arm::UpdateEfectorPos()
{
	effector_point = GetEffectorPoint();
}

void Arm::UpdateChild()
{
	const auto& effector_point = GetEffectorPoint();
	child->SetAnchorPos(effector_point);
	child->UpdateEfectorPos();
	child->UpdateChain();
}

void Arm::UpdateChain()
{
	need_update = true;
	if (child)
	{
		UpdateChild();
	}
}

void Arm::Update()
{
	vertices[0] = anchor_point.x;
	vertices[1] = anchor_point.y;
	vertices[2] = anchor_point.z;
	vertices[3] = effector_point.x;
	vertices[4] = effector_point.y;
	vertices[5] = effector_point.z;
}
