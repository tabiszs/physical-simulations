#include "Cube.h"

void Cube::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
}

void Cube::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void Cube::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTraingles((Object*)this, indices.size() - 2, 0);
	diagonal_shader->use();
	device->DrawLines((Object*)this, 2, indices.size() - 2);
}
