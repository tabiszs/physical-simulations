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
		camera->ObjectRotation(0.0f, 0.5f);
		SetDevice(device);
		start_cursor = make_shared<Cursor>(ShaderHolder::Get().initial_cursorShader);
		start_cursor->LoadMeshTo(device);
		start_cursor->position = start_position;
		final_cursor = make_shared<Cursor>(ShaderHolder::Get().final_cursorShader);
		final_cursor->LoadMeshTo(device);
		final_cursor->position = end_position;
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
	void StartAnimation();
	PumaParameters InverseKinematic(std::shared_ptr<Cursor> curor);

	shared_ptr<Cursor> start_cursor;
	shared_ptr<Cursor> final_cursor;
	shared_ptr<Puma> puma_reverse_kinematic;
	shared_ptr<Puma> puma_interpolation;
	shared_ptr<Grid> grid;
	PumaParameters start_params{};
	PumaParameters final_params{};
	PumaParameters interpolation_params{};
	PumaParameters kinematic_params{};

private:
	const float menu_width = 300.0f;
	float time_start, animation_time = 5.0f;
	int frame_count = 100;
	float step{};
	bool pause = false;
	int animation_frame;

	glm::vec3 start_position{ -3.0f, 1.0f, 0.0f };
	glm::vec3 start_euler_angles{};
	glm::quat start_quaternion{};
	glm::vec3 end_position{ 0.0f, 1.0f, 2.0f };
	glm::vec3 end_euler_angles{};
	glm::quat end_quaternion{};
	float l1{ 1 }, l2{ 1 }, l3{ 1 }, l4{ 1 };
	float q1{}, q2{}, q3{}, q4{}, q5{};

	void UpdateAnimation();
	void UpdateInterpolationPuma(float current_frame);
	void SetLeftViewport();
	void SetRightViewport();
	void DrawInitialCursor();
	void DrawFinalCursor();
};



