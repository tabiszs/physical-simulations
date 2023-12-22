#pragma once
#include "scene.h"
#include "Cursor.h"
#include "Grid.h"

class PumaScene : public Scene
{
public:
	PumaScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{
		camera->Rotate(0.0f, 0.5f);
		SetDevice(device);
		euler_cursor = make_shared<Cursor>(ShaderHolder::Get().euler_cursorShader);
		euler_cursor->LoadMeshTo(device);
		quat_cursor = make_shared<Cursor>(ShaderHolder::Get().quat_cursorShader);
		quat_cursor->LoadMeshTo(device);
		initial_cursor = make_shared<Cursor>(ShaderHolder::Get().initial_cursorShader);
		initial_cursor->LoadMeshTo(device);
		initial_cursor->position = { -1.0f, 0.0f, 0.0f };
		final_cursor = make_shared<Cursor>(ShaderHolder::Get().final_cursorShader);
		final_cursor->LoadMeshTo(device);
		final_cursor->position = { 1.0f, 2.0f, 2.0f };

		// add left puma
		// add right puma
		// add plane
		grid = make_shared<Grid>();
		grid->LoadMeshTo(device);
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
	shared_ptr<Grid> grid;
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
	void UpdateQuaternionInterpolation(double current_time);
	void SetLeftViewport();
	void SetRightViewport();
	void DrawInitialCursor();
	void DrawFinalCursor();
};



