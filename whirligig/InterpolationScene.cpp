#include "InterpolationScene.h"
#include "Quaternion.h"

void InterpolationScene::SetViewport(float width, float height)
{
	// finally don't needed
	//glViewportIndexedf(0, menu_width, 0, (width- menu_width)/2, height);
	//glViewportIndexedf(1, (width - menu_width) / 2, 0, width / 2, height);
}

void InterpolationScene::UpdateViewFrustum(int width, int height)
{
	viewFrustrum->setViewportSize({ long((width - menu_width)/2.0f), height });
}

void InterpolationScene::PerformAllFramesOn(std::shared_ptr<Device> device)
{
	DrawInitialCursor();
	for (int i = 0; i < frame_count; ++i)
	{
		auto simulated_time_from_start = animation_time * i / frame_count;
		UpdateEulerInterpolation(simulated_time_from_start);
		UpdateQuaternionInterpolation(simulated_time_from_start);
		UpdateObjects();

		SetLeftViewport();
		quat_cursor->DrawModelOn(device);
			
		SetRightViewport();
		euler_cursor->DrawModelOn(device);	
	}
	DrawFinalCursor();
}

void InterpolationScene::PerformOneFrameOn(std::shared_ptr<Device> device)
{
	SetLeftViewport();
	initial_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	if (draw_animation)
		quat_cursor->DrawModelOn(device);

	SetRightViewport();
	initial_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	if (draw_animation)
		euler_cursor->DrawModelOn(device);
}

void InterpolationScene::DrawOn(std::shared_ptr<Device> device)
{
	if (draw_all_frames)
	{
		PerformAllFramesOn(device);
	}
	else
	{
		PerformOneFrameOn(device);
	}
}

void InterpolationScene::Update()
{
	if (draw_all_frames) return;

	if (start) {
		UpdateInterpolation();
	}

	UpdateObjects();
}

void InterpolationScene::Menu()
{
	ImGui::SetNextWindowSize({ menu_width, viewFrustrum->getHeight() }, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0.0f, 0.0f }, ImGuiCond_Always);
	ImGui::Begin("Interpolation Movement", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	ImGui::Text("Controls");
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, draw_all_frames);
	if (ImGui::Button("Start")) {
		if (!draw_animation)
		{
			euler_cursor->SetPosition(initial_cursor->position);
			euler_cursor->SetRotation(initial_cursor->euler_angles);
			euler_cursor->SetRotation(initial_cursor->quaternion);

			quat_cursor->SetPosition(initial_cursor->position);
			quat_cursor->SetRotation(initial_cursor->euler_angles);
			quat_cursor->SetRotation(initial_cursor->quaternion);
		}
		start = true;
		draw_animation = true;
		step = animation_time / frame_count;
		final_cursor->ImproveShortestPath(initial_cursor->euler_angles);
		time_start = time = glfwGetTime();
		dt = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		start = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		start = false;
		draw_animation = false;		
	}
	ImGui::PopItemFlag();
	ImGui::SliderInt("Speed", &speed, 1, 100);
	ImGui::SliderFloat("Animation time", &animation_time, 1.0f, 10.0f);
	ImGui::SliderInt("Frame count", &frame_count, 1, 100);
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
	ImGui::Checkbox("Show all frames", &draw_all_frames);
	ImGui::PopItemFlag();
	ImGui::Separator();

	ImGui::Text("Initial configuration");
	initial_cursor->need_update |= ImGui::InputFloat3("Position##0", (float*)&initial_cursor->position);
	ImGui::Text("Euler Angles");
	bool initial_euler_angles_changed = false;
	initial_euler_angles_changed |= ImGui::SliderAngle("X##0", &initial_cursor->euler_angles[0], -180.0f, 180.0f, "%.0f deg", ImGuiSliderFlags_NoInput);
	initial_euler_angles_changed |= ImGui::SliderAngle("Y##0", &initial_cursor->euler_angles[1], -180.0f, 180.0f, "%.0f deg", ImGuiSliderFlags_NoInput);
	initial_euler_angles_changed |= ImGui::SliderAngle("Z##0", &initial_cursor->euler_angles[2], -180.0f, 180.0f, "%.0f deg", ImGuiSliderFlags_NoInput);
	if (initial_euler_angles_changed) 
		initial_cursor->SetQuaternion(initial_cursor->euler_angles);
	ImGui::Text("Quaternion");
	bool initial_quaternion_changed = ImGui::InputFloat4("Qxyzw##0", (float*)&initial_cursor->quaternion);
	if (initial_quaternion_changed) 
		initial_cursor->SetEulerAngles(initial_cursor->quaternion);
	ImGui::Separator();

	ImGui::Text("Final configuration"); 
	final_cursor->need_update |= ImGui::InputFloat3("Position##1", (float*)&final_cursor->position);
	ImGui::Text("Euler Angles"); 
	bool final_euler_angles_changed = false;
	final_euler_angles_changed |= ImGui::SliderAngle("X##1", &final_cursor->euler_angles[0], -180.0f, 180.0f, "%.0f deg", ImGuiSliderFlags_NoInput);
	final_euler_angles_changed |= ImGui::SliderAngle("Y##1", &final_cursor->euler_angles[1], -180.0f, 180.0f, "%.0f deg", ImGuiSliderFlags_NoInput);
	final_euler_angles_changed |= ImGui::SliderAngle("Z##1", &final_cursor->euler_angles[2], -180.0f, 180.0f, "%.0f deg", ImGuiSliderFlags_NoInput);
	if (final_euler_angles_changed)
		final_cursor->SetQuaternion(final_cursor->euler_angles);
	ImGui::Text("Quaternion");
	bool final_quaternion_changed = ImGui::InputFloat4("Qxyzw##1", (float*)&final_cursor->quaternion);
	if (final_quaternion_changed)
		final_cursor->SetEulerAngles(final_cursor->quaternion);
	ImGui::Separator();
	ImGui::Text("Quaternion Interpolation:");
	ImGui::RadioButton("LERP", &use_slerp_quaternion_interpolation, 0);
	ImGui::RadioButton("SLERP", &use_slerp_quaternion_interpolation, 1);
	ImGui::Separator();
	ImGui::End();
}

void InterpolationScene::UpdateObjects()
{
	std::shared_ptr<Shader> shader;

	if (euler_cursor->need_update)
	{
		shader = euler_cursor->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", euler_cursor->ModelMatrix());
		euler_cursor->need_update = false;
	}

	if (quat_cursor->need_update)
	{
		shader = quat_cursor->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", quat_cursor->ModelMatrixQuat());
		quat_cursor->need_update = false;
	}

	if (initial_cursor->need_update)
	{
		shader = initial_cursor->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", initial_cursor->ModelMatrix());
		initial_cursor->need_update = false;
	}

	if (final_cursor->need_update)
	{
		shader = final_cursor->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", final_cursor->ModelMatrix());
		final_cursor->need_update = false;
	}

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		SetProjViewMtx(euler_cursor->shader);
		SetProjViewMtx(initial_cursor->shader);
		SetProjViewMtx(final_cursor->shader);
		SetProjViewMtx(quat_cursor->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void InterpolationScene::UpdateInterpolation()
{
	auto current_time = glfwGetTime();
	auto time_from_start = current_time - time_start;
	dt += (current_time - time) * speed;
	if (time_from_start - dt <= animation_time)
	{
		while (dt > step)
		{
			UpdateEulerInterpolation(time_from_start);
			UpdateQuaternionInterpolation(time_from_start);
			dt -= step;
		}		
	}
	else
	{
		draw_animation = false;
	}
	time = current_time;
}

void InterpolationScene::UpdateEulerInterpolation(double time_from_start)
{
	float animation_part = std::min(1.0, time_from_start / animation_time);
	euler_cursor->position[0] = initial_cursor->position[0] * (1 - animation_part) + final_cursor->position[0] * animation_part;
	euler_cursor->position[1] = initial_cursor->position[1] * (1 - animation_part) + final_cursor->position[1] * animation_part;
	euler_cursor->position[2] = initial_cursor->position[2] * (1 - animation_part) + final_cursor->position[2] * animation_part;

	euler_cursor->euler_angles[0] = initial_cursor->euler_angles[0] * (1 - animation_part) + final_cursor->euler_angles[0] * animation_part;
	euler_cursor->euler_angles[1] = initial_cursor->euler_angles[1] * (1 - animation_part) + final_cursor->euler_angles[1] * animation_part;
	euler_cursor->euler_angles[2] = initial_cursor->euler_angles[2] * (1 - animation_part) + final_cursor->euler_angles[2] * animation_part;

	euler_cursor->need_update = true;
}

void InterpolationScene::UpdateQuaternionInterpolation(double time_from_start)
{
	float animation_part = std::min(1.0, time_from_start / animation_time);
	quat_cursor->position[0] = initial_cursor->position[0] * (1 - animation_part) + final_cursor->position[0] * animation_part;
	quat_cursor->position[1] = initial_cursor->position[1] * (1 - animation_part) + final_cursor->position[1] * animation_part;
	quat_cursor->position[2] = initial_cursor->position[2] * (1 - animation_part) + final_cursor->position[2] * animation_part;

	if (use_slerp_quaternion_interpolation)
	{
		quat_cursor->quaternion = Quaternion::SLERP(initial_cursor->quaternion, final_cursor->quaternion, animation_part);
	}
	else
	{
		quat_cursor->quaternion = Quaternion::LERP(initial_cursor->quaternion, final_cursor->quaternion, animation_part);
	}

	quat_cursor->need_update = true;
}

void InterpolationScene::SetLeftViewport()
{
	glViewport(menu_width, 0, viewFrustrum->getWidth(), viewFrustrum->getHeight());
}

void InterpolationScene::SetRightViewport()
{
	glViewport(menu_width + viewFrustrum->getWidth(), 0, viewFrustrum->getWidth(), viewFrustrum->getHeight());
}

void InterpolationScene::DrawInitialCursor()
{
	SetLeftViewport();
	initial_cursor->DrawModelOn(device);
	SetRightViewport();
	initial_cursor->DrawModelOn(device);
}

void InterpolationScene::DrawFinalCursor()
{
	SetLeftViewport();
	final_cursor->DrawModelOn(device);
	SetRightViewport();
	final_cursor->DrawModelOn(device);
}