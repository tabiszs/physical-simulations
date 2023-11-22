#include "JellyScene.h"

void JellyScene::DrawOn(std::shared_ptr<Device> device)
{
}

void JellyScene::Update()
{
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

	ImGui::End();
}
