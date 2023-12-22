#pragma once
#include "scene.h"
#include "Cube.h"
#include "ControlCube.h"
#include "Jelly.h"

class JellyScene : public Scene
{
public:
	JellyScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{		
		SetDevice(device);
		bounding_cube = make_shared<Cube>(5.0f);
		bounding_cube->LoadMeshTo(device);
		control_cube = make_shared<ControlCube>();
		control_cube->LoadMeshTo(device);
		jelly = make_shared<Jelly>();
		jelly->LoadMeshTo(device);
		light = make_shared<Light>(glm::vec3(0, 3, 0));
		const auto& shader = jelly->bezier_cube->shader;
		shader->use();
		shader->set3Float("lightPosition", light->position);
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

	shared_ptr<Cube> bounding_cube;
	shared_ptr<ControlCube> control_cube;
	shared_ptr<Jelly> jelly;
	shared_ptr<Light> light;
	

private:
	void UpdateJelly();
	void UpdateTrajectory();
	bool m_show_jelly_points = true, m_show_jelly_edges = true, m_show_bezier_cube = true;
};

