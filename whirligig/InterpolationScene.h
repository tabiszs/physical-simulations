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

	shared_ptr<Cursor> euler_cursor;
	shared_ptr<Cursor> quat_cursor;
	shared_ptr<Cursor> initial_cursor;
	shared_ptr<Cursor> final_cursor;
private:
	const float menu_width = 300.0f;
	float time_start, animation_time = 5.0f;
	int frame_count = 20;
	float step{};
	bool draw_animation = false;
	int use_slerp_quaternion_interpolation = 0;

	void UpdateInterpolation();
	void UpdateEulerInterpolation(double current_time);
	void UpdateQuaternionInterpolation(double current_time);
};

