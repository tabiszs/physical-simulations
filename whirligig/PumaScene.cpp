#include "PumaScene.h"
#include "Quaternion.h"
#include <glm/gtc/matrix_access.hpp>

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
	start_params = InverseKinematicFor(start_cursor);
	SetGuiParams(start_params);
	final_params = InverseKinematicFor(final_cursor);

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

void PumaScene::SetGuiParams(const PumaParameters& pp)
{
	q1 = pp.q1;
	q2 = pp.q2;
	q3 = pp.q3;
	q4 = pp.q4;
	l2 = pp.l2;
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
				UpdateKinematicPuma((float)animation_frame);
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

void PumaScene::UpdateKinematicPuma(float current_frame)
{
	// TODO
}

PumaParameters PumaScene::InverseKinematicFor(std::shared_ptr<Cursor> cursor)
{
	const glm::mat4 frame = cursor->GetFrame();
	const glm::vec3 axisZ = glm::column(frame, 2);
	const glm::vec3 effector_pos = glm::column(frame, 3);

	glm::vec3 p0, p1, p2, p3, p4;
	const auto l4 = puma_reverse_kinematic->arm4->GetHeight();
	const auto l3 = puma_reverse_kinematic->arm3->GetHeight();
	p0 = initial_puma_pos;
	p4 = effector_pos;
	p1 = puma_reverse_kinematic->arm1->GetEndPoint();
	p3 = p4 - axisZ * l4;

	// compute p2
	const auto p43 = p4 - p3;
	const auto n = glm::cross(p1 - p3, p1 - p0);
	// jesli wspolliniowe -> zamiast p3 wez p4 -> nie wsm to cos prostrzego -> mozna juz cos powiedziec od p2

	// jakie warunki, zeby moc obliczac
	auto fun_p2y = [p3, p43, n](float p2z) {
		float mianownik = (p43.y * n.x - n.y * p43.x);
		mianownik = abs(mianownik) < FLT_EPSILON ? FLT_EPSILON : mianownik;
		return p3.y + (p2z - p3.z) * (n.z * p43.x - p43.z * n.x) / mianownik;
	};
	auto fun_p2x = [p3, p43](float p2y, float p2z) {
		float p43x = abs(p43.x) < FLT_EPSILON ? FLT_EPSILON : p43.x;
		return p3.x + ((p2y - p3.y) * p43.y + (p2z - p3.z) * p43.z) / p43x;
	};

	float mb = n.x * n.x * p3.z * (p43.y * p43.y + p43.z * p43.z) - 2 * p43.x * n.x * p3.z * (p43.y * n.y + p43.z * n.z) + p3.z * ((n.y * p43.z - p43.y * n.z) * (n.y * p43.z - p43.y * n.z) + p43.x * p43.x * (n.y * n.y + n.z * n.z));
	float licznik = l3 * l3 * (n.x * n.x * (p43.y * p43.y + p43.z * p43.z) + (n.y * p43.z - p43.y * n.z) * (n.y * p43.z - p43.y * n.z) - 2 * p43.x * n.x * (p43.y * n.y + p43.z * n.z) + p43.x * p43.x * (n.y * n.y + n.z * n.z));
	float mianownik = (n.x * p43.y - p43.x * n.y) * (n.x * p43.y - p43.x * n.y);
	mianownik = mianownik < FLT_EPSILON ? FLT_EPSILON : mianownik; // TODO ??? CHECK
	float sqt = std::sqrtf(licznik / mianownik);
	float sqrt_delta = n.x * n.x * p43.y * p43.y * sqt - 2 * p43.x * n.x * p43.y * n.y * sqt + p43.x * p43.x * n.y * n.y * sqt;
	float two_a = n.x * n.x * (p43.y * p43.y + p43.z * p43.z) + (n.y * p43.z - p43.y * n.z) * (n.y * p43.z - p43.y * n.z) - 2 * p43.x * n.x * (p43.y * n.y + p43.z * n.z) + p43.x * p43.x * (n.y * n.y + n.z * n.z);
	float z2_1 = (mb + sqrt_delta) / two_a;
	float z2_2 = (mb - sqrt_delta) / two_a;
	float y2_1 = fun_p2y(z2_1);
	float y2_2 = fun_p2y(z2_2);
	float x2_1 = fun_p2x(y2_1, z2_1);
	float x2_2 = fun_p2x(y2_1, z2_2);
	float x2_3 = fun_p2x(y2_2, z2_1);
	float x2_4 = fun_p2x(y2_2, z2_2);
	
	glm::vec3 p2_1 = { 0, 1, 1 };
	//glm::vec3 p2_1 = { x2_1, y2_2, z2_2 };
	//glm::vec3 p2_1 = { x2_1, y2_1, z2_1 };
	//glm::vec3 p2_2 = { x2_2, y2_2, z2_2 };

		// + rozne warianty p2

	// mamy pkt 
	// testowo -3,1,1 i 180stZ
	// wyznaczamy parametry

	return SetParametersFromPoints(p0, p1, p2_1, p3, p4, frame);
}

PumaParameters PumaScene::SetParametersFromPoints(const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec3 p4, const glm::mat4 effector_frame)
{
	PumaParameters pp{};
	const auto p43 = p4 - p3;
	const auto n = glm::cross(p1 - p3, p1 - p0);
	const glm::vec3 axisX = glm::column(effector_frame, 0);

	// q1
	pp.q1 = atan2f(-n.z, n.x); // for each position give good solution
	// const glm::vec3 default_a1_dir = { 1,0,0 };
	// auto a1 = acosf(glm::dot(default_a1_dir, n) / (1 * glm::l2Norm(n)));

	// q2
	const auto v21 = p2 - p1;
	const auto F01 = Mat::rotationY(pp.q1);
	const glm::vec3 v_f2 = F01 * glm::vec4(v21, 0);
	pp.q2 = atan2f(v_f2.y, v_f2.z);
	//const auto v21_2 = p2_2 - p1;
	//const float a2_x = glm::length(glm::vec2(v21.x, v21.z));
	//const float a2 = atan2f(v21.y, a2_x);

	// l3
	pp.l2 = glm::length(v21);

	// q3
	const auto v32 = glm::normalize(p3 - p2);
	const auto F02 = Mat::rotationX(-pp.q2) * F01;
	const glm::vec3 v_f3 = F02 * glm::vec4(v32, 0);
	pp.q3 = atan2f(-v_f3.z, -v_f3.y);

	// q4
	const auto F03 = Mat::rotationX(-pp.q3) * F02;
	const glm::vec3 v_f4 = F03 * glm::vec4(p43, 0);
	pp.q4 = atan2f(v_f4.x, v_f4.z);

	// q5
	const auto F04 = Mat::rotationY(pp.q4) * F02;
	const glm::vec3 v_f5 = F04 * glm::vec4(axisX, 0);
	pp.q5 = atan2f(v_f5.y, v_f5.x);
	return pp;
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