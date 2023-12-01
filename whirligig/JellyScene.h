#pragma once
#include "scene.h"
#include "Cube.h"
#include "ControlCube.h"
#include "BezierCube.h"

class JellyScene : public Scene
{
public:
	JellyScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
		: Scene(c, l, vf)
	{	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

	shared_ptr<Cube> bounding_cube;
	shared_ptr<ControlCube> control_cube;
	shared_ptr<BezierCube> bezier_cube;
	
	

private:
	void UpdateJelly();
	void UpdateTrajectory();
	bool m_show_jelly_points = true, m_show_jelly_edges = true, m_show_bezier_cube = true;
};

