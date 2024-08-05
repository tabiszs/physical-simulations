#include "ScreenPlane.h"

void ScreenPlane::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", color);
}

void ScreenPlane::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void ScreenPlane::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTriangles((Object*)this);
}
