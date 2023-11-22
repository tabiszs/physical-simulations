#include "WhirligigScene.h"

void WhirligigScene::DrawOn(std::shared_ptr<Device> device)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	if (showPlane)
		plane->DrawModelOn(device);
	if (showCube)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cube->DrawModelOn(device);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (showDiagonal)
		cube->DrawDiagonalOn(device);
	if (showTrajectory)
		trajectory->DrawModelOn(device);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void WhirligigScene::Update()
{
	std::shared_ptr<Shader> shader;

	if (start) {
		UpdateTrajectory();
	}

	if (trajectory->need_update)
	{
		trajectory->UpdateMeshTo(device);
		trajectory->need_update = false;
	}

	if (cube->need_update)
	{
		shader = cube->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", cube->ModelMatrix());

		shader = cube->diagonal_shader;
		shader->use();
		shader->setMatrix4F("modelMtx", cube->ModelMatrix());
		cube->need_update = false;
	}

	if (plane->need_update)
	{
		shader = plane->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", plane->ModelMatrix());
		plane->need_update = false;
	}

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		SetProjViewMtx(plane->shader);
		SetProjViewMtx(cube->shader);
		SetProjViewMtx(cube->diagonal_shader);
		SetProjViewMtx(trajectory->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void WhirligigScene::Menu()
{
	ImGui::Begin("Whirligig");
	ImGui::Text("Controls");
	if (ImGui::Button("Start")) {
		start = true;
		this->cube->SetProperties();
		this->trajectory->Clean();
		time = glfwGetTime();
		dt = 0;
	}
	if (ImGui::Button("Stop")) {
		start = false;
	}
	if (ImGui::Button("Reset")) {
		start = false;
		this->cube->SetProperties();
		this->trajectory->Clean();
	}
	ImGui::Separator();

	ImGui::Text("Components");
	ImGui::Checkbox("Cube", &this->showCube);
	ImGui::Checkbox("Cube Diagonal", &this->showDiagonal);
	ImGui::Checkbox("Cube Trajectory", &this->showTrajectory);
	ImGui::Checkbox("Ground Plane", &this->showPlane);
	ImGui::Separator();

	ImGui::Text("Initial Conditions");
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
	ImGui::SliderFloat("Cube size", &this->cube->maxSize, 1.0f, 2.0f);
	ImGui::SliderFloat("Cube density", &this->cube->density, 0.1f, 10.f);
	if (ImGui::SliderAngle("Cube inflection", &this->cube->inflection, 0.0f, 90.0f))
		this->cube->ChangedInflection();
	ImGui::SliderFloat("Cube angular momentum", &(this->cube->velocity), 0, 10); // rad/s w kierunku y na scenie
	ImGui::SliderInt("Trajectory length", &(this->trajectory->length), 1000, this->trajectory->max_trajectory);
	ImGui::PopItemFlag();
	ImGui::Checkbox("Use gravitation", &this->cube->use_gravitation);
	ImGui::SliderInt("Speed", &speed, 1, 100);
	ImGui::SliderFloat("Step", &this->cube->dt, 0.001f, 1.0f);
	ImGui::End();
}

void WhirligigScene::UpdateTrajectory()
{
	auto time2 = glfwGetTime();
	dt += time2 - time;
	time = time2;

	dt *= speed;
	auto step = cube->dt;
	while (dt > step)
	{
		cube->Update(step);
		glm::vec3 cone = cube->GetConeCoordinates();
		trajectory->push_back(cone);
		dt -= step;
	}
}
