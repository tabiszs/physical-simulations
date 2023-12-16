#include "JellyScene.h"

void JellyScene::DrawOn(std::shared_ptr<Device> device)
{
	bounding_cube->DrawModelOn(device);
	control_cube->DrawModelOn(device);
	if (m_show_bezier_cube)
		jelly->DrawModelOn(device);
	if (m_show_jelly_points)
		jelly->DrawPointsOn(device);
	if (m_show_jelly_edges)
		jelly->DrawEdgesOn(device);
}

void JellyScene::Update()
{
	UpdateJelly();

	std::shared_ptr<Shader> shader;

	if (bounding_cube->need_update)
	{
		shader = bounding_cube->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", bounding_cube->ModelMatrix());
		bounding_cube->need_update = false;
	}

	if (control_cube->need_update)
	{
		shader = control_cube->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", control_cube->ModelMatrix());
		control_cube->need_update = false;
	}

	if (jelly->need_update)
	{
		jelly->UpdateMeshTo(device);
		jelly->need_update = false;
	}

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		SetProjViewMtx(bounding_cube->shader);
		SetProjViewMtx(control_cube->shader);
		SetProjViewMtx(jelly->shader);
		SetProjViewMtx(jelly->bezier_cube->shader);
		SetInvViewMtx(jelly->bezier_cube->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void JellyScene::Menu()
{
	ImGui::Begin("Jelly");
	ImGui::Text("Controls");
	if (ImGui::Button("Start")) {
		start = true;
		/*whirligig_scene->cube->SetProperties();
		whirligig_scene->trajectory->Clean();*/
		time = glfwGetTime();
		dt = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		start = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		start = false;
		/*whirligig_scene->cube->SetProperties();
		whirligig_scene->trajectory->Clean();*/
	}
	ImGui::SliderInt("Speed", &speed, 1, 100);
	ImGui::SliderFloat("Step", &this->jelly->dt, 0.001f, 1.0f);
	ImGui::Separator();

	ImGui::Text("Components");
	ImGui::Checkbox("Draw jelly points", &m_show_jelly_points);
	ImGui::Checkbox("Draw jelly edges", &m_show_jelly_edges);
	ImGui::Checkbox("Draw bezier cube", &m_show_bezier_cube);
	ImGui::Separator();

	jelly->need_update |= ImGui::InputFloat3("Gravitation", jelly->gravitation);
	// mu?
	ImGui::Text("Velocity after collision type");
	jelly->need_update |= ImGui::RadioButton("One component", &jelly->use_whole_vector_velocities, 0);
	jelly->need_update |= ImGui::RadioButton("Whole vector", &jelly->use_whole_vector_velocities, 1);
	jelly->need_update |= ImGui::SliderFloat("mass", &jelly->mass, 1.0f, 10.0f);
	jelly->need_update |= ImGui::SliderFloat("k", &jelly->k, 1.0f, 10.0f);
	jelly->need_update |= ImGui::SliderFloat("c1", &jelly->c1, 0.0f, 50.0f);
	jelly->need_update |= ImGui::SliderFloat("c2", &jelly->c2, 0.0f, 50.0f);
	control_cube->need_update |= ImGui::InputFloat3("Control Cube Position", control_cube->position);
	control_cube->need_update |= ImGui::InputFloat3("Control Cube Rotation", control_cube->euler_angles);

	ImGui::End();
}

void JellyScene::UpdateJelly()
{
	auto time2 = glfwGetTime();
	dt += time2 - time;
	time = time2;

	dt *= speed;
	auto step = jelly->dt;
	while (dt > step)
	{
		auto cc_corners = control_cube->GetCornersPositions();
		jelly->ComputeForce(cc_corners);
		auto bb_dim = bounding_cube->GetDimensions();
		jelly->TakeCollisionsIntoAccount(bb_dim);
		dt -= step;
	}
}