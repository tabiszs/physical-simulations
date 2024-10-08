#pragma once
#include "scene.h"
#include "Cursor.h"
class InterpolationScene : public Scene
{
public:
	InterpolationScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{	
		SetDevice(device);
		euler_cursor = make_shared<Cursor>(ShaderHolder::Get().euler_cursorShader);
		euler_cursor->LoadMeshTo(device);
		quat_cursor = make_shared<Cursor>(ShaderHolder::Get().quat_cursorShader);
		quat_cursor->LoadMeshTo(device);
		initial_cursor = make_shared<Cursor>(ShaderHolder::Get().initial_cursorShader);
		initial_cursor->LoadMeshTo(device);
		final_cursor = make_shared<Cursor>(ShaderHolder::Get().final_cursorShader);
		final_cursor->LoadMeshTo(device);
		final_cursor->position[2] = -0.5f;
		final_cursor->position[2] = 2.0f;
	}

	void SetViewport(float width, float height) override;
	void UpdateViewFrustum(int width, int height) override;
	void PerformAllFramesOn(std::shared_ptr<Device> device);
	void PerformOneFrameOn(std::shared_ptr<Device> device);
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
	int frame_count = 10;
	float step{};
	bool draw_animation = false;
	bool draw_all_frames = false;
	int use_slerp_quaternion_interpolation = 0;

	void UpdateObjects();
	void UpdateInterpolation();
	void UpdateEulerInterpolation(double current_time);
	void UpdateQuaternionInterpolation(double current_time);
	void SetLeftViewport();
	void SetRightViewport();
	void DrawInitialCursor();
	void DrawFinalCursor();
};

