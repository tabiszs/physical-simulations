#include "Line.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 Line::ModelMatrix()
{
	auto r = glm::toMat4(quaternion);
	auto t = Mat::translation(position);
	auto s = Mat::scale(scale);
	return t * r * s;
}

void Line::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", (float*)&color);
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void Line::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
	shader->use();
	shader->setMatrix4F("modelMtx", ModelMatrix());
	need_update = false;
}

void Line::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawLines((Object*)this);
}

void Line::Rotate(glm::vec3 euler_angles)
{
	quaternion *= glm::quat(euler_angles);
	need_update = true;
}

void Line::Translate(glm::vec3 t)
{
	position += t;
	need_update = true;
}

void Line::Scale(float s)
{
	scale = s;
	need_update = true;
}

void Line::SetEndPositions(glm::vec3 p0, glm::vec3 p1)
{
	vertices[0] = p0.x;
	vertices[1] = p0.y;
	vertices[2] = p0.z;

	vertices[3] = p1.x;
	vertices[4] = p1.y;
	vertices[5] = p1.z;

	need_update = true;
}
