#pragma once
#include "scene.h"
#include "Cursor.h"
#include "Grid.h"
#include "Puma.h"
#include "Cubemap.h"

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

		cubemap = make_shared<Cubemap>(ShaderHolder::Get().blackHoleShader, faces);
		cubemap->LoadMeshTo(device);
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

private:
	std::shared_ptr<Cubemap> cubemap;

	unsigned int cubemapTexture;

	const float menu_width = 300.0f;
	const float menu_height = 100.0f;

	const float min_distance = 300;
	const float max_distance = 1000;
	float distance = 300;

	const float min_mass = 1;
	const float max_mass = 100;
	float mass = 1;
};