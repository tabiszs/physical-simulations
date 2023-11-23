#pragma once
#include "scene.h"

class WhirligigScene: public Scene
{
public:
	WhirligigScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
		: Scene(c, l, vf)
	{	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

	shared_ptr<WhirligigCube> cube;
	shared_ptr<Plane> plane;
	shared_ptr<Trajectory> trajectory;
	bool showCube = true, showTrajectory = true, showDiagonal = true, showPlane = true;

private:
	void UpdateTrajectory();
};

