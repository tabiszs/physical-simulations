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
	if(start)
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
		time = glfwGetTime();
		dt = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		start = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		control_cube->Restart();
		jelly->Restart();
		start = false;
	}
	ImGui::SliderInt("Speed", &speed, 1, 100);
	ImGui::SliderFloat("Step", &this->jelly->dt, 0.001f, 1.0f);
	ImGui::Separator();

	ImGui::Text("Components");
	ImGui::Checkbox("Draw jelly points", &m_show_jelly_points);
	ImGui::Checkbox("Draw jelly edges", &m_show_jelly_edges);
	ImGui::Checkbox("Draw bezier cube", &m_show_bezier_cube);
	ImGui::Separator();

	jelly->need_update |= ImGui::InputFloat3("Gravitation", (float*) &jelly->gravitation);
	jelly->need_update |= ImGui::SliderFloat("Rebound factor", &jelly->rebound_factor, 0.0f, 1.0f);
	ImGui::Text("Velocity after collision type");
	jelly->need_update |= ImGui::RadioButton("One component", &jelly->use_whole_vector_velocities, 0);
	jelly->need_update |= ImGui::RadioButton("Whole vector", &jelly->use_whole_vector_velocities, 1);
	jelly->need_update |= ImGui::SliderFloat("mass per point", &jelly->mass, 1.0f, 10.0f);
	jelly->need_update |= ImGui::SliderFloat("k", &jelly->k, 1.0f, 20.0f);
	jelly->need_update |= ImGui::SliderFloat("c1", &jelly->c1, 0.0f, 100.0f);
	jelly->need_update |= ImGui::SliderFloat("c2", &jelly->c2, 0.0f, 100.0f);
	jelly->need_update |= ImGui::Checkbox("Include Control Cube", &jelly->include_control_cube);
	control_cube->need_update |= ImGui::InputFloat3("Control Cube Position", (float*) &control_cube->position);
	control_cube->need_update |= ImGui::InputFloat3("Control Cube Rotation", (float*) &control_cube->euler_angles);
	if (ImGui::Button("Pinch 5th point")) // wychyl 5 pkt z polozenia rownowagi
	{
		jelly->Pinch(5);
	}
	ImGui::SliderFloat("deviation value", &jelly->pinch, 0, 2);
	if (ImGui::Button("Punch 5th point")) // nadaj 5 pkt predkosc poczatkowa
	{
		jelly->Punch(5);
	}
	ImGui::SliderFloat("Initial velocity", &jelly->punch, 0, 2);

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
		jelly->IncludeCollisions(bb_dim);
		dt -= step;
	}
}