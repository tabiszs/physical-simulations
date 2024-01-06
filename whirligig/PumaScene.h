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
		interpolation_cursor = make_shared<Cursor>(ShaderHolder::Get().NewCursorShader());
		start_cursor = make_shared<Cursor>(ShaderHolder::Get().initial_cursorShader);
		start_cursor->LoadMeshTo(device);
		start_cursor->position = start_position;
		final_cursor = make_shared<Cursor>(ShaderHolder::Get().final_cursorShader);
		final_cursor->LoadMeshTo(device);
		final_cursor->position = end_position;
		puma_interpolation = make_shared<Puma>();
		puma_interpolation->LoadMeshTo(device);
		puma_kinematic = make_shared<Puma>();
		puma_kinematic->LoadMeshTo(device);
		grid = make_shared<Grid>();
		grid->LoadMeshTo(device);

		start_params = InverseKinematicFor(start_cursor);
		final_params = InverseKinematicFor(final_cursor);
		puma_interpolation->SetParams(start_params);
		puma_kinematic->SetParams(start_params);
		SetGuiParams(start_params);
	}

	void SetViewport(float width, float height) override;
	void UpdateViewFrustum(int width, int height) override;
	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;
	void StartAnimation();
	void ProcessKeyCallback(GLFWwindow* m_Window) override;

	shared_ptr<Cursor> start_cursor;
	shared_ptr<Cursor> final_cursor;
	shared_ptr<Puma> puma_kinematic;
	shared_ptr<Puma> puma_interpolation;
	shared_ptr<Grid> grid;
	PumaParameters start_params{};
	PumaParameters final_params{};
	PumaParameters interpolation_params{};
	PumaParameters kinematic_params{};
	shared_ptr<Cursor> interpolation_cursor;

private:
	const float menu_width = 300.0f;
	float time_start, animation_time = 5.0f;
	int frame_count = 100;
	float step{};
	bool pause = false;
	int animation_frame;

	glm::vec3 start_position{ -2.0f, 1.0f, 1.0f };
	glm::vec3 start_euler_angles{};
	glm::quat start_quaternion = { 1,0,0,0 };
	glm::vec3 end_position{ 0.0f, 1.0f, 2.0f };
	glm::vec3 end_euler_angles{};
	glm::quat end_quaternion = { 1,0,0,0 };
	float l1{ 1 }, l2{ 1 }, l3{ 1 }, l4{ 1 };
	float q1{}, q2{}, q3{}, q4{}, q5{};
	const glm::vec4 initial_puma_pos{};
	glm::vec3 last_p2{};

	void SetGuiParams(const PumaParameters& pp);
	void UpdateAnimation();
	void UpdateInterpolationPuma(float current_frame);
	void UpdateKinematicPuma(float current_frame);
	PumaParameters InverseKinematicFor(std::shared_ptr<Cursor> cursor);
	std::pair<glm::vec3, glm::vec3> SetP2(const glm::vec3 p3, const glm::vec3 p4);
	PumaParameters SetParametersFromPoints(const glm::vec3 p0, 
		const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, 
		const glm::vec3 p4, const glm::mat4 effector_frame);
	void SetLeftViewport();
	void SetRightViewport();
	void DrawInitialCursor();
	void DrawFinalCursor();
};



