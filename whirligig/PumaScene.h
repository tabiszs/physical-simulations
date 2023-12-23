#pragma once
#include "scene.h"
#include "Cursor.h"
#include "Grid.h"
#include "Puma.h"

class PumaScene : public Scene
{
public:
	PumaScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{
		camera->Rotate(0.0f, 0.5f);
		SetDevice(device);
		cursor_interpolation = make_shared<Cursor>(ShaderHolder::Get().euler_cursorShader);
		cursor_interpolation->LoadMeshTo(device);
		cursor_reverse = make_shared<Cursor>(ShaderHolder::Get().quat_cursorShader);
		cursor_reverse->LoadMeshTo(device);
		initial_cursor = make_shared<Cursor>(ShaderHolder::Get().initial_cursorShader);
		initial_cursor->LoadMeshTo(device);
		initial_cursor->position = { -1.0f, 0.0f, 0.0f };
		final_cursor = make_shared<Cursor>(ShaderHolder::Get().final_cursorShader);
		final_cursor->LoadMeshTo(device);
		final_cursor->position = { 1.0f, 2.0f, 2.0f };
		puma_interpolation = make_shared<Puma>();
		puma_interpolation->LoadMeshTo(device);
		puma_reverse_kinematic = make_shared<Puma>();
		puma_reverse_kinematic->LoadMeshTo(device);
		grid = make_shared<Grid>();
		grid->LoadMeshTo(device);
	}

	void SetViewport(float width, float height) override;
	void UpdateViewFrustum(int width, int height) override;
	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;

	shared_ptr<Cursor> cursor_interpolation;
	shared_ptr<Cursor> cursor_reverse;
	shared_ptr<Cursor> initial_cursor;
	shared_ptr<Cursor> final_cursor;
	shared_ptr<Puma> puma_reverse_kinematic;
	shared_ptr<Puma> puma_interpolation;
	shared_ptr<Grid> grid;
private:
	const float menu_width = 300.0f;
	float time_start, animation_time = 5.0f;
	int frame_count = 10;
	float step{};
	bool draw_animation = false;

	glm::vec3 start_position{};
	glm::vec3 start_euler_angles{};
	glm::quat start_quaternion{};
	glm::vec3 end_position{};
	glm::vec3 end_euler_angles{};
	glm::quat end_quaternion{};
	float l1{ 1 }, l3{ 1 }, l4{ 1 }, q1{}, q2{};

	void UpdateInterpolation();
	void UpdateQuaternionInterpolation(double current_time);
	void SetLeftViewport();
	void SetRightViewport();
	void DrawInitialCursor();
	void DrawFinalCursor();
};



