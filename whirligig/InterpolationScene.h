#pragma once
#include "scene.h"
#include "Cursor.h"
class InterpolationScene : public Scene
{
public:
	InterpolationScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
		: Scene(c, l, vf)
	{	
		//camera_movement = false;
	}

	void SetViewport(float width, float height) override;
	void UpdateViewFrustum(int width, int height) override;
	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

	shared_ptr<Cursor> cursorQ;

private:
	const float menu_width = 300.0f;
};

