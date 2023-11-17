#include "Plane.h"

void Plane::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
}

void Plane::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void Plane::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTraingles((Object*)this);
}
