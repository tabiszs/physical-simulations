#include "Cube.h"

void Cube::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", color);
	diagonal_shader->use();
	diagonal_shader->set4Float("objectColor", diagonal_color);
}

void Cube::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void Cube::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTraingles((Object*)this, indices.size() - 2, 0);
}

void Cube::DrawDiagonalOn(std::shared_ptr<Device> device)
{
	diagonal_shader->use();
	device->DrawLines((Object*)this, 2, indices.size() - 2);
}