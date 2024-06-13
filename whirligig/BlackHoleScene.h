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

		std::vector<std::string> space_texture
		{
			"textures/GalaxyTex_PositiveX.png",
			"textures/GalaxyTex_NegativeX.png",
			"textures/GalaxyTex_PositiveY.png",
			"textures/GalaxyTex_NegativeY.png",
			"textures/GalaxyTex_PositiveZ.png",
			"textures/GalaxyTex_NegativeZ.png",
		};

		std::vector<std::string> debug_texture
		{
			"textures/tex_positiveX.png",
			"textures/tex_negativeX.png",
			"textures/tex_positiveY.png",
			"textures/tex_negativeY.png",
			"textures/tex_positiveZ.png",
			"textures/tex_negativeZ.png",
		};

		const auto& shader = ShaderHolder::Get().blackHoleShader;
		plane = make_shared<ScreenPlane>(shader);
		plane->LoadMeshTo(device);

		space_texture_id = device->LoadCubemap(space_texture);
		debug_texture_id = device->LoadCubemap(debug_texture);

		shader->use();
		SetInvProjViewMtx(shader);
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

private:
	std::shared_ptr<ScreenPlane> plane;

	const float menu_width = 300.0f;
	const float menu_height = 180.0f;

	unsigned int space_texture_id;
	unsigned int debug_texture_id;
	bool show_debug_texture = false;
	bool texture_changed = true;

	bool orbiting_camera = false;
	bool orbiting_camera_changed = true;

	glm::vec3 blackHolePosition = { 0, 0, 0 };
	const float distance_coefficient = 1e6; // 1000 km
	const float min_distance = 200.0f;
	const float max_distance = 1000;
	float distance = min_distance;
	bool distance_changed = true;

	const int min_mass_coefficient = 1;
	const int max_mass_coefficient = 100;	
	const float sun_mass = 1.9891e30f; // kg
	const float being747_mass = 200e3f; // kg - explicity: 184 t
	const float mass() const { return being747_mass * mass_coefficient; }
	int mass_coefficient = 1;	
	bool mass_changed = true;
};