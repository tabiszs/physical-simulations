#pragma once
#include "scene.h"
#include "Hodograph.h"
#include "Line.h"
class HodographScene : public Scene
{
public:
	HodographScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{
		camera_movement = true;
		this->device = device;
		hodograph = make_shared<Hodograph>();
		hodograph->LoadMeshTo(device);
		lineX = make_shared<Line>(-100.0f, 100.0f, red);
		lineX->LoadMeshTo(device);
		lineY = make_shared<Line>(-100.0f, 100.0f, green);
		lineY->Rotate({ 0,0,glm::half_pi<float>() });
		lineY->LoadMeshTo(device);
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;


	const float menu_width = 300.0f;
	float time_start, animation_time = 5.0f;
	int frame_count = 100;
	float step{};
	bool pause = false;
	int animation_frame;
private:
	std::shared_ptr<Hodograph> hodograph;
	std::shared_ptr<Line> lineX;
	std::shared_ptr<Line> lineY;
	void StartAnimation();
	void UpdateAnimation();
};

