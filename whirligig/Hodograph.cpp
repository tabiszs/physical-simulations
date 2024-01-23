#include "Hodograph.h"

void Hodograph::Init()
{
	PoC = { R, 0 };
	auto x = R + L;
	PoP = { x, 0 };
	dPoP = {};
	ddPoP = {};
	total_rad = 0;

	idx = 0;
	time = 0;
	plot_size = 0;

	UpdateRadianStep();
	UpdateLines();
	circle->Scale(R);
}

void Hodograph::Update()
{
	// update angle
	total_rad += rad_step;
	if (total_rad > glm::two_pi<float>())
	{
		total_rad -= glm::two_pi<float>();
	}

	PositionPoC();

	// PoP position
	// + epsilon
	glm::vec2 new_PoP{ PoC.x + sqrtf(L * L - PoC.y * PoC.y), 0 };

	// numerical derivative computation
	glm::vec2 new_dPoP = (new_PoP - PoP) / step;
	glm::vec2 new_ddPoP = (new_dPoP - dPoP) / step;

	// update buffers
	PoP = new_PoP;
	dPoP = new_dPoP;
	ddPoP = new_ddPoP;

	times[idx] = time;
	positions[idx] = PoP.x;
	velocities[idx] = dPoP.x;
	accelerations[idx] = ddPoP.x;

	// update indices
	idx = (++idx) % SIZE;
	time += step;
	if (plot_size < SIZE)
	{
		++plot_size;
	}

	UpdateLines();
}

void Hodograph::UpdateL()
{
	PositionPoP();
	UpdateLines();
}

void Hodograph::UpdateR()
{
	PositionPoC();
	PositionPoP();

	UpdateLines();
	circle->Scale(R);
}

void Hodograph::LoadMeshTo(std::shared_ptr<Device> device)
{
	lineL->LoadMeshTo(device);
	lineR->LoadMeshTo(device);
	circle->LoadMeshTo(device);
}

void Hodograph::UpdateMeshTo(std::shared_ptr<Device> device)
{
	if (lineL->need_update)
	{
		lineL->UpdateMeshTo(device);
	}
	if (lineR->need_update)
	{
		lineR->UpdateMeshTo(device);
	}
	if (circle->need_update)
	{
		circle->UpdateMeshTo(device);
	}
}

void Hodograph::DrawModelOn(std::shared_ptr<Device> device)
{
	circle->DrawModelOn(device);
	lineL->DrawModelOn(device);
	lineR->DrawModelOn(device);
}

void Hodograph::UpdateLines()
{
	lineL->SetEndPositions(glm::vec3(PoC, 0), glm::vec3(PoP, 0));
	lineR->SetEndPositions({ 0,0,0 }, glm::vec3(PoC, 0));
}

void Hodograph::PositionPoC()
{
	float x = R * cosf(total_rad);
	float y = R * sinf(total_rad);
	PoC = { x,y };
}

void Hodograph::PositionPoP()
{
	PoP = { PoC.x + sqrtf(L * L - PoC.y * PoC.y), 0 };
}
