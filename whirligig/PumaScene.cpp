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
	start_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	puma_interpolation->DrawModelOn(device);		

	SetRightViewport();
	grid->DrawModelOn(device);
	start_cursor->DrawModelOn(device);
	final_cursor->DrawModelOn(device);
	puma_reverse_kinematic->DrawModelOn(device);
}

void PumaScene::Update()
{
	if (start) 
	{
		UpdateAnimation();
	}

	std::shared_ptr<Shader> shader;

	puma_interpolation->Update(device);
	puma_reverse_kinematic->Update(device);

	if (start_cursor->need_update)
	{
		shader = start_cursor->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", start_cursor->ModelMatrix());
		start_cursor->need_update = false;
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

		SetProjViewMtx(start_cursor->shader);
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
	ImGui::Text("Controls");
	{
		if (ImGui::Button("Start")) {
			StartAnimation();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			pause = !pause;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			start = pause = false;
			puma_interpolation->SetParams(start_params);
			puma_reverse_kinematic->SetParams(start_params);
		}
		ImGui::SliderInt("Speed", &speed, 1, 100);
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
		ImGui::SliderFloat("Animation time", &animation_time, 1.0f, 10.0f);
		ImGui::SliderInt("Frame count", &frame_count, 1, 100);
		ImGui::Separator();
	}

	ImGui::Text("Position");
	{
		if (ImGui::InputFloat3("Start##0", (float*)&start_position))
		{
			start_cursor->SetPosition(start_position);
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
			start_cursor->SetQuaternion(start_quaternion);
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
			start_cursor->SetEulerAngles(start_euler_angles);
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
		/*if (ImGui::InputFloat("l2", &l2, 0.1f))
		{
			puma_interpolation->SetLengthOfArm2(l2);
			puma_reverse_kinematic->SetLengthOfArm2(l2);
		}*/
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
		/*if (ImGui::InputFloat("q1", &q1, 0.1f))
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
		}*/
		ImGui::Separator();
	}
	ImGui::PopItemFlag();
	ImGui::End();
}

void PumaScene::StartAnimation()
{
	final_params.l2 = 2;
	final_params.q1 = 1;
	final_params.q2 = 2;
	final_params.q3 = 1;
	final_params.q4 = 2;
	// popraw katy do zakresu gwaratujacego najkrotsza interpolacje
	// odwrotna kinematyka dla pozycji startowej i koncowej ->
	
	start = true;
	pause = false;
	dt = 0;
	animation_frame = 0;
	time_start = time = glfwGetTime();
	puma_interpolation->SetParams(start_params);
	puma_reverse_kinematic->SetParams(start_params);
}

PumaParameters PumaScene::InverseKinematic(std::shared_ptr<Cursor> curor)
{
	return PumaParameters();
}

void PumaScene::UpdateAnimation()
{
	auto current_time = glfwGetTime();
	auto time_from_start = current_time - time_start;
	auto dfl_dt = (current_time - time);
	if (pause)
	{
		time_start += dfl_dt;
	}
	else
	{
		dt += dfl_dt * speed;
	}

	if (start && (time_from_start - dfl_dt) <= animation_time)
	{
		const auto& step = animation_time / frame_count;
		while (dt > step)
		{
			if (animation_frame < frame_count)
			{
				UpdateInterpolationPuma((float)animation_frame);
				//UpdateKinematicPuma(time_from_start);
				animation_frame++;
			}
			dt -= step;
		}
	}
	else
	{
		start = pause = false;
	}
	time = current_time;
}

void PumaScene::UpdateInterpolationPuma(float current_frame)
{
	float animation_part = std::min(1.0f, current_frame / frame_count);
	interpolation_params.Interpolation(start_params, final_params, animation_part);
	puma_interpolation->SetParams(interpolation_params);
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
	start_cursor->DrawModelOn(device);
	SetRightViewport();
	start_cursor->DrawModelOn(device);
}

void PumaScene::DrawFinalCursor()
{
	SetLeftViewport();
	final_cursor->DrawModelOn(device);
	SetRightViewport();
	final_cursor->DrawModelOn(device);
}