#include "PumaScene.h"
#include "Quaternion.h"

void PumaScene::SetViewport(float width, float height)
{
	// finally don't needed
	//glViewportIndexedf(0, menu_width, 0, (width- menu_width)/2, height);
	//glViewportIndexedf(1, (width - menu_width) / 2, 0, width / 2, height);
}

void PumaScene::UpdateViewFrustum(int width, int height)
{
	viewFrustrum->setViewportSize({ long((width - menu_width) / 2.0f), height });
}

void PumaScene::DrawOn(std::shared_ptr<Device> device)
{
	SetLeftViewport();
	grid->DrawModelOn(device);
	initial_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	puma_interpolation->DrawModelOn(device); // TODO - remove
	if (draw_animation)
	{
		// TODO
		//cursor_interpolation->DrawModelOn(device);
		//puma_interpolation->DrawModelOn(device);
	}
		

	SetRightViewport();
	grid->DrawModelOn(device);
	initial_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	if (draw_animation)
	{
		// TODO
		//cursor_reverse->DrawModelOn(device);
		//puma_reverse_kinematic->DrawModelOn(device);
	}		
}

void PumaScene::Update()
{
	if (start) 
	{
		UpdateInterpolation();
	}

	std::shared_ptr<Shader> shader;

	puma_interpolation->Update(device);
	puma_reverse_kinematic->Update(device);

	if (cursor_interpolation->need_update)
	{
		shader = cursor_interpolation->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", cursor_interpolation->ModelMatrix());
		cursor_interpolation->need_update = false;
	}

	if (cursor_reverse->need_update)
	{
		shader = cursor_reverse->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", cursor_reverse->ModelMatrixQuat());
		cursor_reverse->need_update = false;
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

		SetProjViewMtx(cursor_interpolation->shader);
		SetProjViewMtx(initial_cursor->shader);
		SetProjViewMtx(final_cursor->shader);
		SetProjViewMtx(cursor_reverse->shader);
		SetProjViewMtx(grid->shader);

		SetProjViewMtx(puma_interpolation->joint1->shader);
		SetProjViewMtx(puma_interpolation->joint2->shader);
		SetProjViewMtx(puma_interpolation->joint3->shader);
		SetProjViewMtx(puma_interpolation->joint4->shader);
		SetProjViewMtx(puma_interpolation->arm1->shader);
		SetProjViewMtx(puma_interpolation->arm2->shader);
		SetProjViewMtx(puma_interpolation->arm3->shader);
		SetProjViewMtx(puma_interpolation->arm4->shader);

		SetProjViewMtx(puma_reverse_kinematic->joint1->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint2->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint3->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint4->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm1->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm2->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm3->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm4->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void PumaScene::Menu()
{
	ImGui::SetNextWindowSize({ menu_width, viewFrustrum->getHeight() }, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0.0f, 0.0f }, ImGuiCond_Always);
	ImGui::Begin("Puma Kinematics", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	ImGui::Text("Controls");
	if (ImGui::Button("Start")) {
		if (!draw_animation)
		{
			cursor_interpolation->SetPosition(initial_cursor->position);
			cursor_interpolation->SetRotation(initial_cursor->euler_angles);
			cursor_interpolation->SetRotation(initial_cursor->quaternion);

			cursor_reverse->SetPosition(initial_cursor->position);
			cursor_reverse->SetRotation(initial_cursor->euler_angles);
			cursor_reverse->SetRotation(initial_cursor->quaternion);
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
	ImGui::SliderInt("Speed", &speed, 1, 100);
	ImGui::SliderFloat("Animation time", &animation_time, 1.0f, 10.0f);
	ImGui::SliderInt("Frame count", &frame_count, 1, 100); // TODO
	ImGui::Separator();

	ImGui::Text("Position");
	if (ImGui::InputFloat3("Start##0", (float*)&start_position))
	{

	}
	if (ImGui::InputFloat3("End##0", (float*)&end_position))
	{

	}
	ImGui::Separator();

	ImGui::Text("Quaternion");
	if (ImGui::SliderFloat4("Start##1", (float*)&start_quaternion, -1, 1))
	{

	}
	if (ImGui::SliderFloat4("End##1", (float*)&end_quaternion, -1, 1))
	{

	}
	if (ImGui::Button("Display##1"))
	{

	}
	ImGui::Separator();

	ImGui::Text("Euler angles");
	if (ImGui::SliderFloat3("Start##2", (float*)&start_euler_angles, -glm::pi<float>(), glm::pi<float>()))
	{

	}
	if (ImGui::SliderFloat3("End##2", (float*)&end_euler_angles, -glm::pi<float>(), glm::pi<float>()))
	{

	}
	if (ImGui::Button("Display##2"))
	{

	}
	ImGui::Separator();

	ImGui::Text("Puma");
	ImGui::InputFloat("l1", &l1, 0.1f);
	ImGui::InputFloat("l3", &l3, 0.1f);
	ImGui::InputFloat("l4", &l4, 0.1f);
	ImGui::InputFloat("q1", &q1, 0.1f);
	ImGui::InputFloat("q2", &q2, 0.1f);
	ImGui::Separator();
	ImGui::End();
}

void PumaScene::UpdateInterpolation()
{
	auto current_time = glfwGetTime();
	auto time_from_start = current_time - time_start;
	dt += (current_time - time) * speed;
	if (time_from_start - dt <= animation_time)
	{
		while (dt > step)
		{
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

void PumaScene::UpdateQuaternionInterpolation(double time_from_start)
{
	float animation_part = std::min(1.0, time_from_start / animation_time);
	cursor_reverse->position = initial_cursor->position * (1 - animation_part) + final_cursor->position * animation_part;
	cursor_reverse->quaternion = glm::normalize(glm::slerp(initial_cursor->quaternion, final_cursor->quaternion, animation_part));
	cursor_reverse->need_update = true;
}

void PumaScene::SetLeftViewport()
{
	glViewport(menu_width, 0, viewFrustrum->getWidth(), viewFrustrum->getHeight());
}

void PumaScene::SetRightViewport()
{
	glViewport(menu_width + viewFrustrum->getWidth(), 0, viewFrustrum->getWidth(), viewFrustrum->getHeight());
}

void PumaScene::DrawInitialCursor()
{
	SetLeftViewport();
	initial_cursor->DrawModelOn(device);
	SetRightViewport();
	initial_cursor->DrawModelOn(device);
}

void PumaScene::DrawFinalCursor()
{
	SetLeftViewport();
	final_cursor->DrawModelOn(device);
	SetRightViewport();
	final_cursor->DrawModelOn(device);
}