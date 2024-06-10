#pragma once
#include "scene.h"
#include "Cursor.h"
#include "Grid.h"
#include "Puma.h"
#include "Cubemap.h"
#include "ScreenPlane.h"

class BlackHoleScene : public Scene
{
public:
	BlackHoleScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{
		glEnable(GL_DEPTH_TEST);
		SetDevice(device);

		std::vector<std::string> faces = std::vector<std::string>
		{
			"textures/GalaxyTex_PositiveX.png",
			"textures/GalaxyTex_NegativeX.png",
			"textures/GalaxyTex_PositiveY.png",
			"textures/GalaxyTex_NegativeY.png",
			"textures/GalaxyTex_PositiveZ.png",
			"textures/GalaxyTex_NegativeZ.png",
		};

		const auto& shader = ShaderHolder::Get().blackHoleShader;
		plane = make_shared<ScreenPlane>(shader);
		plane->LoadMeshTo(device);

		cubemap = make_shared<Cubemap>(shader, faces);
		cubemap->LoadMeshTo(device);

		blackHolePosition.z = distance * distance_coefficient;
		shader->use();

		shader->set3Float("blackHolePosition", blackHolePosition);
		shader->setFloat("mass", mass());
		shader->setFloat("distance", distance);
		SetInvProjViewMtx(shader);		
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

private:
	std::shared_ptr<Cubemap> cubemap;
	std::shared_ptr<ScreenPlane> plane;

	const float menu_width = 300.0f;
	const float menu_height = 120.0f;

	glm::vec3 blackHolePosition = { 0, 0, 1e9f };
	const float distance_coefficient = 1e6; // 1000 km
	const float min_distance = 300;
	const float max_distance = 1000;
	float distance = 300;
	bool distance_changed = true; //initial value

	const int min_mass_coefficient = 1;
	const int max_mass_coefficient = 100;	
	const float sun_mass = 1.9891e30f; // kg
	const float being747_mass = 200e3f; // kg - explicity: 184 t
	const float mass() const { return being747_mass * mass_coefficient; }
	int mass_coefficient = 1;	
	bool mass_changed = true; //initial value
	
};