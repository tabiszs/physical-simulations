#include "JellyScene.h"

void JellyScene::DrawOn(std::shared_ptr<Device> device)
{
	box->DrawModelOn(device);
	control_frame->DrawModelOn(device);
}

void JellyScene::Update()
{
	std::shared_ptr<Shader> shader;

	if (box->need_update)
	{
		shader = box->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", box->ModelMatrix());
		box->need_update = false;
	}

	if (control_frame->need_update)
	{
		shader = control_frame->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", control_frame->ModelMatrix());
		control_frame->need_update = false;
	}

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		SetProjViewMtx(box->shader);
		SetProjViewMtx(control_frame->shader);

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
	//ImGui::SliderFloat("Step", &this->cube->dt, 0.001f, 1.0f);
	ImGui::Separator();
	
	control_frame->need_update |= ImGui::InputFloat3("Control Frame Position", control_frame->position);
	control_frame->need_update |= ImGui::InputFloat3("Control Frame Rotation", control_frame->euler_angles);

	ImGui::End();
}
