#include "PumaEffector.h"
#include <glm/gtx/quaternion.hpp>
#include "Cylinder.h"

glm::mat4 PumaEffector::ModelMatrix() const
{
	float s = 1.0f / (maxPt - minPt);
	auto scale = Mat::scale(s, s, s);
	auto t = Mat::translation({ position[0],position[1] ,position[2] });
	auto r = glm::toMat4(quaternion);
	glm::mat4 transformation = t * r * scale;
	if (parent != nullptr)
	{
		transformation = parent->Frame() * transformation;
	}
	return transformation;
}

void PumaEffector::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadPositionsAndColor((Object*)this);
	shader->use();
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void PumaEffector::UpdateTo(std::shared_ptr<Device> device)
{
	shader->use();
	shader->setMatrix4F("modelMtx", ModelMatrix());
	need_update = false;
}
