#include "Puma.h"

void Puma::LoadMeshTo(std::shared_ptr<Device> device)
{
	joint1->LoadMeshTo(device);
	joint2->LoadMeshTo(device);
	joint3->LoadMeshTo(device);
	joint4->LoadMeshTo(device);
	joint5->LoadMeshTo(device);
	arm1->LoadMeshTo(device);
	arm2->LoadMeshTo(device);
	arm3->LoadMeshTo(device);
	arm4->LoadMeshTo(device);
	effector->LoadMeshTo(device);
}

void Puma::Update(std::shared_ptr<Device> device)
{
	if (joint1->need_update) joint1->UpdateMeshTo(device);
	if (joint2->need_update) joint2->UpdateMeshTo(device);
	if (joint3->need_update) joint3->UpdateMeshTo(device);
	if (joint4->need_update) joint4->UpdateMeshTo(device);
	if (joint5->need_update) joint5->UpdateMeshTo(device);
	if (arm1->need_update) arm1->UpdateMeshTo(device);
	if (arm2->need_update) arm2->UpdateMeshTo(device);
	if (arm3->need_update) arm3->UpdateMeshTo(device);
	if (arm4->need_update) arm4->UpdateMeshTo(device);
	if (effector->need_update) effector->UpdateTo(device);
}

void Puma::DrawModelOn(std::shared_ptr<Device> device)
{
	joint1->DrawModelOn(device);
	joint2->DrawModelOn(device);
	joint3->DrawModelOn(device);
	joint4->DrawModelOn(device);
	joint5->DrawModelOn(device);
	arm1->DrawModelOn(device);
	arm2->DrawModelOn(device);
	arm3->DrawModelOn(device);
	arm4->DrawModelOn(device);
	effector->DrawModelOn(device);
}

void Puma::SetLengthOfArm1(float l)
{
	l1 = l;
	arm1->SetHeight(l);
}

void Puma::SetLengthOfArm2(float l)
{
	l2 = l;
	arm2->SetHeight(l);
}

void Puma::SetLengthOfArm3(float l)
{
	l3 = l;
	arm3->SetHeight(l);
}

void Puma::SetLengthOfArm4(float l)
{
	l4 = l;
	arm4->SetHeight(l);
}

void Puma::SetAngleOfJoint1(float r)
{
	q1 = r;
	joint1->SetAngle(r);
}

void Puma::SetAngleOfJoint2(float r)
{
	q2 = r;
	joint2->SetAngle(r);
}

void Puma::SetAngleOfJoint3(float r)
{
	q3 = r;
	joint3->SetAngle(r);
}

void Puma::SetAngleOfJoint4(float r)
{
	q4 = r;
	joint4->SetAngle(r);
}

void Puma::SetAngleOfJoint5(float r)
{
	q5 = r;
	joint5->SetAngle(r);
}

void Puma::SetParams(PumaParameters pp)
{
	SetLengthOfArm2(pp.l2);
	SetAngleOfJoint1(pp.q1);
	SetAngleOfJoint2(pp.q2);
	SetAngleOfJoint3(pp.q3);
	SetAngleOfJoint4(pp.q4);
	SetAngleOfJoint5(pp.q5);
}
