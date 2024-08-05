#pragma once
#include "scene.h"

class WhirligigScene: public Scene
{
public:
	WhirligigScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{	
		SetDevice(device);
		cube = make_shared<WhirligigCube>();
		cube->LoadMeshTo(device);
		plane = make_shared<Plane>();
		plane->LoadMeshTo(device);
		trajectory = make_shared<Trajectory>();
		trajectory->LoadMeshTo(device);
	}

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

