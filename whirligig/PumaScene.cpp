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
	puma_kinematic->DrawModelOn(device);
}

void PumaScene::Update()
{
	if (start) 
	{
		UpdateAnimation();
	}

	std::shared_ptr<Shader> shader;

	puma_interpolation->Update(device);
	puma_kinematic->Update(device);

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

		SetProjViewMtx(puma_kinematic->joint1->shader);
		SetProjViewMtx(puma_kinematic->joint2->shader);
		SetProjViewMtx(puma_kinematic->joint3->shader);
		SetProjViewMtx(puma_kinematic->joint4->shader);
		SetProjViewMtx(puma_kinematic->joint5->shader);
		SetProjViewMtx(puma_kinematic->arm1->shader);
		SetProjViewMtx(puma_kinematic->arm2->shader);
		SetProjViewMtx(puma_kinematic->arm3->shader);
		SetProjViewMtx(puma_kinematic->arm4->shader);
		SetProjViewMtx(puma_kinematic->effector->shader);

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
			puma_kinematic->SetParams(start_params);
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
		}
		ImGui::Separator();
	}

	ImGui::Text("Euler angles");
	{
		if (ImGui::SliderFloat3("Start##2", (float*)&start_euler_angles, -glm::pi<float>(), glm::pi<float>()))
		{
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);

			start_params = InverseKinematicFor(start_cursor);
			puma_interpolation->SetParams(start_params);
			puma_kinematic->SetParams(start_params);
			SetGuiParams(start_params);
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
			puma_kinematic->SetLengthOfArm1(l1);
		}
		if (ImGui::InputFloat("l2", &l2, 0.1f))
		{
			puma_interpolation->SetLengthOfArm2(l2);
			puma_kinematic->SetLengthOfArm2(l2);
		}
		if (ImGui::InputFloat("l3", &l3, 0.1f))
		{
			puma_interpolation->SetLengthOfArm3(l3);
			puma_kinematic->SetLengthOfArm3(l3);
		}
		if (ImGui::InputFloat("l4", &l4, 0.1f))
		{
			puma_interpolation->SetLengthOfArm4(l4);
			puma_kinematic->SetLengthOfArm4(l4);
		}
		if (ImGui::InputFloat("q1", &q1, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint1(q1);
			puma_kinematic->SetAngleOfJoint1(q1);
		}
		if (ImGui::InputFloat("q2", &q2, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint2(q2);
			puma_kinematic->SetAngleOfJoint2(q2);
		}
		if (ImGui::InputFloat("q3", &q3, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint3(q3);
			puma_kinematic->SetAngleOfJoint3(q3);
		}
		if (ImGui::InputFloat("q4", &q4, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint4(q4);
			puma_kinematic->SetAngleOfJoint4(q4);
		}
		if (ImGui::InputFloat("q5", &q5, 0.1f))
		{
			puma_interpolation->SetAngleOfJoint5(q5);
			puma_kinematic->SetAngleOfJoint5(q5);
		}
		ImGui::Separator();
	}
	ImGui::PopItemFlag();
	ImGui::End();
}

void PumaScene::StartAnimation()
{
	ReduceAngles();
	final_params = InverseKinematicFor(final_cursor);
	start_params = InverseKinematicFor(start_cursor);
	ReduceParameters();
	puma_interpolation->SetParams(start_params);
	puma_kinematic->SetParams(start_params);
	SetGuiParams(start_params);

	start = true;
	pause = false;
	dt = 0;
	animation_frame = 0;
	time_start = time = glfwGetTime();
	puma_interpolation->SetParams(start_params);
	puma_kinematic->SetParams(start_params);
}

void PumaScene::ProcessKeyCallback(GLFWwindow* m_Window)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureKeyboard)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = std::min(currentFrame - lastFrame, 0.1f);
		lastFrame = currentFrame;
		const auto position = start_cursor->position;
		const auto euler_angles = start_euler_angles;
		bool pressed = false;
		
		if (glfwGetKey(m_Window, GLFW_KEY_R) == GLFW_PRESS)
		{
			start_position += glm::vec3(1, 0, 0) * deltaTime * MOVEMENT_SPEED;
			start_cursor->SetPosition(start_position);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_PRESS)
		{
			start_position += glm::vec3(-1, 0, 0) * deltaTime * MOVEMENT_SPEED;
			start_cursor->SetPosition(start_position);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_T) == GLFW_PRESS)
		{
			start_position += glm::vec3(0, 1, 0) * deltaTime * MOVEMENT_SPEED;
			start_cursor->SetPosition(start_position);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_G) == GLFW_PRESS)
		{
			start_position += glm::vec3(0, -1, 0) * deltaTime * MOVEMENT_SPEED;
			start_cursor->SetPosition(start_position);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_Y) == GLFW_PRESS)
		{
			start_position += glm::vec3(0, 0, 1) * deltaTime * MOVEMENT_SPEED;
			start_cursor->SetPosition(start_position);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_H) == GLFW_PRESS)
		{
			start_position += glm::vec3(0, 0, -1) * deltaTime * MOVEMENT_SPEED;
			start_cursor->SetPosition(start_position);
			pressed = true;
		}


		if (glfwGetKey(m_Window, GLFW_KEY_U) == GLFW_PRESS)
		{
			start_euler_angles += glm::vec3(1, 0, 0) * deltaTime * OBJECT_ROTATION_SPEED;
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_J) == GLFW_PRESS)
		{
			start_euler_angles += glm::vec3(-1, 0, 0) * deltaTime * OBJECT_ROTATION_SPEED;
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_I) == GLFW_PRESS)
		{
			start_euler_angles += glm::vec3(0, 1, 0) * deltaTime * OBJECT_ROTATION_SPEED;
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_K) == GLFW_PRESS)
		{
			start_euler_angles += glm::vec3(0, -1, 0) * deltaTime * OBJECT_ROTATION_SPEED;
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_O) == GLFW_PRESS)
		{
			start_euler_angles += glm::vec3(0, 0, 1) * deltaTime * OBJECT_ROTATION_SPEED;
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);
			pressed = true;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_L) == GLFW_PRESS)
		{
			start_euler_angles += glm::vec3(0, 0, -1) * deltaTime * OBJECT_ROTATION_SPEED;
			start_quaternion = glm::quat(start_euler_angles);
			start_cursor->SetEulerAngles(start_euler_angles);
			pressed = true;
		}

		if (pressed)
		{
			start_params = InverseKinematicFor(start_cursor);
			puma_interpolation->SetParams(start_params);
			puma_kinematic->SetParams(start_params);
			SetGuiParams(start_params);
		}
	}
}

void PumaScene::SetGuiParams(const PumaParameters& pp)
{
	q1 = pp.q1;
	q2 = pp.q2;
	q3 = pp.q3;
	q4 = pp.q4;
	q5 = pp.q5;
	l2 = pp.l2;
}

void PumaScene::ReduceAngles()
{
	auto euler_angles = end_euler_angles;
	auto d_angle = euler_angles - start_euler_angles;
	if (abs(d_angle.x) > glm::pi<float>())
	{
		euler_angles.x += euler_angles.x > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	if (abs(d_angle.y) > glm::pi<float>())
	{
		euler_angles.y += euler_angles.y > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	if (abs(d_angle.z) > glm::pi<float>())
	{
		euler_angles.z += euler_angles.z > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	final_cursor->SetEulerAngles(euler_angles);
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
	float fraction = std::min(1.0f, current_frame / frame_count);
	interpolation_params.Interpolation(start_params, final_params, fraction);
	puma_interpolation->SetParams(interpolation_params);
}

void PumaScene::UpdateKinematicPuma(float current_frame)
{
	float fraction = std::min(1.0f, current_frame / frame_count);
	const auto position = glm::lerp(start_position, end_position, fraction);
	const auto rotation = glm::slerp(start_quaternion, end_quaternion, fraction);
	interpolation_cursor->SetPosition(position);
	interpolation_cursor->SetQuaternion(rotation);
	params = InverseKinematicFor(interpolation_cursor);
	puma_kinematic->SetParams(params);
}

PumaParameters PumaScene::InverseKinematicFor(std::shared_ptr<Cursor> cursor)
{
	const glm::mat4 frame = cursor->GetFrame();
	const glm::vec3 axisZ = glm::column(frame, 2);
	const glm::vec3 effector_pos = glm::column(frame, 3);

	glm::vec3 p0, p1, p2, p3, p4;
	const auto l4 = puma_kinematic->arm4->GetHeight();
	p0 = initial_puma_pos;
	p4 = effector_pos;
	p1 = puma_kinematic->arm1->GetEndPoint();
	p3 = p4 - axisZ * l4;

	const auto [p2_1, p2_2] = SetP2(p0, p1, p3, p4);
	const float norm_1 = glm::length(p2_1 - last_p2);
	const float norm_2 = glm::length(p2_2 - last_p2);
	last_p2 = p2 = norm_1 < norm_2 ? p2_1 : p2_2;

	return SetParametersFromPoints(p0, p1, p2, p3, p4, frame);
}

std::pair<glm::vec3, glm::vec3> PumaScene::SetP2(
	const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p3, const glm::vec3 p4)
{
	glm::vec3 p2_1{}, p2_2{};
	float x2_1{}, x2_2{}, y2_1{}, y2_2{}, z2_1, z2_2{};

	// warunki:
	// 1. p0, p1, p2, p3 sa wspolplaszczyznowe
	// 2. p43 i p32 sa prostopadle
	// 3. dlugosc |p3-p2| = l3

	const auto p43 = p4 - p3;

	if (abs(p43.y) < FLT_EPSILON)
	{
		x2_1 = x2_2 = p3.x;
		z2_1 = z2_2 = p3.z;
		y2_1 = p3.y + l3;
		y2_2 = p3.y - l3;
	}
	else
	{
		if (abs(p3.x) < FLT_EPSILON)
		{
			x2_1 = x2_2 = 0;

			const auto W = p3.x * p3.x + p3.y * p3.y + p3.z * p3.z - l3 * l3;
			const auto K = p43.x * p3.x + p43.y * p3.y + p43.z * p3.z;
			const auto a = 1 + (p43.z * p43.z) / (p43.y * p43.y); // >0 only square vaule
			const auto b = 2 * (p43.z * (p3.y / p43.y - 1 / (p43.y * p43.y)) - p3.z);
			const auto c = W + K * K / (p43.y * p43.y) - 2 * K * p3.y / p43.y;
			const auto delta = b * b - 4 * a * c;
			if (delta < FLT_EPSILON)
			{
				return { last_p2 , last_p2 };
			}
			else
			{
				const auto sqrt_delta = sqrtf(delta);
				z2_1 = 0.5 * (-b + sqrt_delta) / a;
				y2_1 = (K - p43.z * z2_1) / p43.y;

				z2_2 = 0.5 * (-b - sqrt_delta) / a;
				y2_2 = (K - p43.z * z2_1) / p43.y;
			}
		}
		else
		{
			const auto W = p3.x * p3.x + p3.y * p3.y + p3.z * p3.z - l3 * l3;
			const auto K = p43.x * p3.x + p43.y * p3.y + p43.z * p3.z;
			const auto P = p43.x + p43.z * p3.z / p3.x;
			const auto a = (1 + P * P / (p43.y * p43.y) + (p3.z * p3.z) / (p3.x * p3.x)); // >0 only square vaule
			const auto b = -2 * p3.x - 2 * K * P / (p43.y * p43.y) + 2 * P * p3.y / p43.y - 2 * p3.z * p3.z / p3.x;
			const auto c = W + K * K / (p43.y * p43.y) - 2 * K * p3.y / p43.y;
			const auto delta = b * b - 4 * a * c;
			if (delta < FLT_EPSILON)
			{
				return { last_p2 , last_p2 };
			}
			else
			{
				const auto sqrt_delta = sqrtf(delta);
				x2_1 = 0.5 * (-b + sqrt_delta) / a;
				y2_1 = (K - x2_1 * P) / p43.y;
				z2_1 = p3.z / p3.x * x2_1;

				x2_2 = 0.5 * (-b - sqrt_delta) / a;
				y2_2 = (K - x2_2 * P) / p43.y;
				z2_2 = p3.z / p3.x * x2_2;
			}
		}
	}
	p2_1 = { x2_1, y2_1, z2_1 };
	p2_2 = { x2_2, y2_2, z2_2 };
	return { p2_1, p2_2 };
}

void PumaScene::ReduceParameters()
{
	auto delta = final_params.q1 - start_params.q1;
	if (abs(delta) > glm::pi<float>())
	{
		final_params.q1 += final_params.q1 > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	delta = final_params.q2 - start_params.q2;
	if (abs(delta) > glm::pi<float>())
	{
		final_params.q2 += final_params.q2 > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	delta = final_params.q3 - start_params.q3;
	if (abs(delta) > glm::pi<float>())
	{
		final_params.q3 += final_params.q3 > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	delta = final_params.q4 - start_params.q4;
	if (abs(delta) > glm::pi<float>())
	{
		final_params.q4 += final_params.q4 > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
	delta = final_params.q5 - start_params.q5;
	if (abs(delta) > glm::pi<float>())
	{
		final_params.q5 += final_params.q5 > 0 ? -glm::two_pi<float>() : glm::two_pi<float>();
	}
}

PumaParameters PumaScene::SetParametersFromPoints(const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec3 p4, const glm::mat4 effector_frame)
{
	PumaParameters pp{};
	const auto p43 = p4 - p3;
	const auto n = glm::cross(p1 - p3, p1 - p0);
	const glm::vec3 axisX = glm::column(effector_frame, 0);

	// q1
	pp.q1 = atan2f(-n.z, n.x); // for each position give good solution

	// q2
	const auto v21 = p2 - p1;
	const auto F01 = Mat::rotationY(pp.q1);
	const glm::vec3 v_f2 = F01 * glm::vec4(v21, 0);
	pp.q2 = atan2f(v_f2.y, v_f2.z);

	// l3
	pp.l2 = glm::length(v21);

	// q3
	const auto v32 = glm::normalize(p3 - p2);
	const auto F02 = Mat::rotationX(pp.q2) * F01;
	const glm::vec3 v_f3 = F02 * glm::vec4(v32, 0);
	pp.q3 = -atan2f(-v_f3.z, -v_f3.y); // zeby obrot byl zgodny z rysunkiem

	// q4
	const auto F03 = Mat::rotationX(pp.q3) * F02;
	const glm::vec3 v_f4 = F03 * glm::vec4(p43, 0);
	pp.q4 = atan2f(v_f4.x, v_f4.z);

	// q5
	const auto F04 = Mat::rotationY(pp.q4) * F03;
	const glm::vec3 v_f5 = F04 * glm::vec4(axisX, 0);
	pp.q5 = atan2f(v_f5.y, v_f5.x); // przesztalcamy uklad zeby pasowal do efektora -> dlatego przeciwny kat	
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