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
	puma_interpolation->DrawModelOn(device);		

	SetRightViewport();
	grid->DrawModelOn(device);
	initial_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	puma_reverse_kinematic->DrawModelOn(device);
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

		SetProjViewMtx(initial_cursor->shader);
		SetProjViewMtx(final_cursor->shader);
		SetProjViewMtx(grid->shader);

		SetProjViewMtx(puma_interpolation->joint1->shader);
		SetProjViewMtx(puma_interpolation->joint2->shader);
		SetProjViewMtx(puma_interpolation->joint3->shader);
		SetProjViewMtx(puma_interpolation->joint4->shader);
		SetProjViewMtx(puma_interpolation->joint5->shader);
		SetProjViewMtx(puma_interpolation->arm1->shader);
		SetProjViewMtx(puma_interpolation->arm2->shader);
		SetProjViewMtx(puma_interpolation->arm3->shader);
		SetProjViewMtx(puma_interpolation->arm4->shader);
		SetProjViewMtx(puma_interpolation->effector->shader);

		SetProjViewMtx(puma_reverse_kinematic->joint1->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint2->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint3->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint4->shader);
		SetProjViewMtx(puma_reverse_kinematic->joint5->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm1->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm2->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm3->shader);
		SetProjViewMtx(puma_reverse_kinematic->arm4->shader);
		SetProjViewMtx(puma_reverse_kinematic->effector->shader);

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
	ImGui::Text("Controls"); // TODO
	{
		if (ImGui::Button("Start")) {
			if (!draw_animation)
			{

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
	}

	ImGui::Text("Position");
	{
		if (ImGui::InputFloat3("Start##0", (float*)&start_position))
		{
			initial_cursor->SetPosition(start_position);
		}
		if (ImGui::InputFloat3("End##0", (float*)&end_position))
		{
			final_cursor->SetPosition(end_position);
		}
		ImGui::Separator();
	}

	ImGui::Text("Quaternion");
	{
		if (ImGui::SliderFloat4("Start##1", (float*)&start_quaternion, -1, 1))
		{
			start_quaternion = glm::normalize(start_quaternion);
			start_euler_angles = glm::eulerAngles(start_quaternion);
			initial_cursor->SetQuaternion(start_quaternion);
		}
		if (ImGui::SliderFloat4("End##1", (float*)&end_quaternion, -1, 1))
		{
			end_quaternion = glm::normalize(end_quaternion);
			end_euler_angles = glm::eulerAngles(end_quaternion);
			final_cursor->SetQuaternion(end_quaternion);
		}
		if (ImGui::Button("Display##1"))
		{
			// start animation with quaternion rotation
			// 1. reduce angles
			// 2. compute inverse kinamatics for initial and final position
			// 3a. interpolate via parameters
			// 3b. interpolate via inverse kinematics
		}
		ImGui::Separator();
	}


	ImGui::Text("Euler angles");
	{
		if (ImGui::SliderFloat3("Start##2", (float*)&start_euler_angles, -glm::pi<float>(), glm::pi<float>()))
		{
			start_quaternion = glm::quat(start_euler_angles);
			initial_cursor->SetEulerAngles(start_euler_angles);
		}
		if (ImGui::SliderFloat3("End##2", (float*)&end_euler_angles, -glm::pi<float>(), glm::pi<float>()))
		{
			end_quaternion = glm::quat(end_euler_angles);
			final_cursor->SetEulerAngles(end_euler_angles);
		}
		if (ImGui::Button("Display##2"))
		{
			// start animation with euler angles rotation
		}
		ImGui::Separator();
	}


	ImGui::Text("Puma");
	{
		if (ImGui::InputFloat("l1", &l1, 0.1f))
		{
			puma_interpolation->SetLengthOfArm1(l1);
			puma_reverse_kinematic->SetLengthOfArm1(l1);
		}
		if (ImGui::InputFloat("l2", &l2, 0.1f))
		{
			puma_interpolation->SetLengthOfArm2(l2);
			puma_reverse_kinematic->SetLengthOfArm2(l2);
		}
		if (ImGui::InputFloat("l3", &l3, 0.1f))
		{
			puma_interpolation->SetLengthOfArm3(l3);
			puma_reverse_kinematic->SetLengthOfArm3(l3);
		}
		if (ImGui::InputFloat("l4", &l4, 0.1f))
		{
			puma_interpolation->SetLengthOfArm4(l4);
			puma_reverse_kinematic->SetLengthOfArm4(l4);
		}
		if (ImGui::InputFloat("q1", &q1, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint1(q1);
			puma_reverse_kinematic->SetAngleOfJoint1(q1);
		}
		if (ImGui::InputFloat("q2", &q2, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint2(q2);
			puma_reverse_kinematic->SetAngleOfJoint2(q2);
		}
		if (ImGui::InputFloat("q3", &q3, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint3(q3);
			puma_reverse_kinematic->SetAngleOfJoint3(q3);
		}
		if (ImGui::InputFloat("q4", &q4, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint4(q4);
			puma_reverse_kinematic->SetAngleOfJoint4(q4);
		}
		if (ImGui::InputFloat("q5", &q5, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint5(q5);
			puma_reverse_kinematic->SetAngleOfJoint5(q5);
		}
		ImGui::Separator();
	}
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
	//cursor_interpolation->position = initial_cursor->position * (1 - animation_part) + final_cursor->position * animation_part;
	//cursor_interpolation->quaternion = glm::normalize(glm::slerp(initial_cursor->quaternion, final_cursor->quaternion, animation_part));
	//cursor_interpolation->need_update = true;
	// puma_interpolation->need_update = true;
	// TODO - update puma
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