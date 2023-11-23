#pragma once
#include "scene.h"
#include "Box.h"
#include "ControlFrame.h"

class JellyScene : public Scene
{
public:
	JellyScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
		: Scene(c, l, vf)
	{	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

	shared_ptr<Box> box;
	shared_ptr<ControlFrame> control_frame;
	bool showCube = true, showTrajectory = true, showDiagonal = true, showPlane = true;

private:
	void UpdateTrajectory();

};

