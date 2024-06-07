#include "Cubemap.h"

void Cubemap::LoadMeshTo(std::shared_ptr<Device> device)
{
	textureId = device->LoadCubemap((Object*)this, faces);
	shader->use();
	auto modelMtx = ModelMatrix();	
	shader->setMatrix4F("modelMtx", modelMtx);
}

void Cubemap::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void Cubemap::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTriangles((Object*)this);

}