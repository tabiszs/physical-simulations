#include "WhirligigCube.h"

void WhirligigCube::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", color);
	diagonal_shader->use();
	diagonal_shader->set4Float("objectColor", diagonal_color);
}

void WhirligigCube::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void WhirligigCube::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTriangles((Object*)this, indices.size() - 2, 0);
}

void WhirligigCube::DrawDiagonalOn(std::shared_ptr<Device> device)
{
	diagonal_shader->use();
	device->DrawLines((Object*)this, 2, indices.size() - 2);
}