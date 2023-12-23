#include "Puma.h"

void Puma::LoadMeshTo(std::shared_ptr<Device> device)
{
	joint1->LoadMeshTo(device);
	joint2->LoadMeshTo(device);
	joint3->LoadMeshTo(device);
	joint4->LoadMeshTo(device);
	arm1->LoadMeshTo(device);
	arm2->LoadMeshTo(device);
	arm3->LoadMeshTo(device);
	arm4->LoadMeshTo(device);
}

void Puma::Update(std::shared_ptr<Device> device)
{
	if (joint1->need_update) joint1->UpdateMeshTo(device);
	if (joint2->need_update) joint2->UpdateMeshTo(device);
	if (joint3->need_update) joint3->UpdateMeshTo(device);
	if (joint4->need_update) joint4->UpdateMeshTo(device);
	if (arm1->need_update) arm1->UpdateMeshTo(device);
	if (arm2->need_update) arm2->UpdateMeshTo(device);
	if (arm3->need_update) arm3->UpdateMeshTo(device);
	if (arm4->need_update) arm4->UpdateMeshTo(device);
}

void Puma::DrawModelOn(std::shared_ptr<Device> device)
{
	joint1->DrawModelOn(device);
	joint2->DrawModelOn(device);
	joint3->DrawModelOn(device);
	joint4->DrawModelOn(device);
	arm1->DrawModelOn(device);
	arm2->DrawModelOn(device);
	arm3->DrawModelOn(device);
	arm4->DrawModelOn(device);
}
