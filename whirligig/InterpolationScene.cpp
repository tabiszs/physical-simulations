#include "InterpolationScene.h"

void InterpolationScene::SetViewport(float width, float height)
{
	// finally don't needed
	//glViewportIndexedf(0, menu_width, 0, (width- menu_width)/2, height);
	//glViewportIndexedf(1, (width - menu_width) / 2, 0, width / 2, height);
}

void InterpolationScene::UpdateViewFrustum(int width, int height)
{
	viewFrustrum->setViewportSize({ long((width - menu_width)/2.0f), height });
}

void InterpolationScene::DrawOn(std::shared_ptr<Device> device)
{
	// one viewport
	glViewport(menu_width, 0, viewFrustrum->getWidth(), viewFrustrum->getHeight());
	cursorQ->DrawModelOn(device);

	// second viewport
	glViewport(menu_width+viewFrustrum->getWidth(), 0, viewFrustrum->getWidth(), viewFrustrum->getHeight());
	cursorQ->DrawModelOn(device);

}

void InterpolationScene::Update()
{
	std::shared_ptr<Shader> shader;

	if (cursorQ->need_update)
	{
		shader = cursorQ->shader;
		shader->use();
		shader->setMatrix4F("modelMtx", cursorQ->ModelMatrix());
	}

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		SetProjViewMtx(cursorQ->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void InterpolationScene::Menu()
{
	ImGui::SetNextWindowSize({ menu_width, viewFrustrum->getHeight() }, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0.0f, 0.0f }, ImGuiCond_Always);
	ImGui::Begin("Interpolation Movement", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
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

	ImGui::Separator();
	ImGui::End();
}
