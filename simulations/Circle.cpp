#include "Circle.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 Circle::ModelMatrix()
{
	auto r = glm::toMat4(quaternion);
	auto t = Mat::translation(position);
	auto s = Mat::scale(scale);
	return t * r * s;
}

void Circle::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", (float*)&color);
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void Circle::UpdateMeshTo(std::shared_ptr<Device> device)
{
	shader->use();
	shader->setMatrix4F("modelMtx", ModelMatrix());
	need_update = false;
}

void Circle::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawLineLoop((Object*)this);
}

void Circle::Rotate(glm::vec3 euler_angles)
{
	quaternion *= glm::quat(euler_angles);
	need_update = true;
}

void Circle::Translate(glm::vec3 t)
{
	position += t;
	need_update = true;
}

void Circle::Scale(float s)
{
	scale = s;
	need_update = true;
}
