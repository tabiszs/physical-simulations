#include <glm/gtc/matrix_access.hpp>
#include "BlackHoleScene.h"


void BlackHoleScene::DrawOn(std::shared_ptr<Device> device)
{
	device->DrawCubemap(cubemap.get(), cubemap->GetTextureId());
}

void BlackHoleScene::Update()
{
	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		const auto viewMtx = glm::mat4(glm::mat3(camera->ViewMatrix()));
		m_viewProjMtx = viewFrustrum->getProjectionMatrix() * viewMtx;
		m_invViewMtx = glm::inverse(viewMtx);
		//UpdateProjViewMtx();
		SetProjViewMtx(cubemap->shader);
	}
}

void BlackHoleScene::Menu()
{
	ImGui::SetNextWindowSize({ menu_width, menu_height }, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0.0f, 0.0f }, ImGuiCond_Always);
	ImGui::Begin("Black Hole", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	ImGui::SliderFloat("Distance", &distance, min_distance, max_distance);
	ImGui::SliderFloat("Mass", &mass, min_mass, max_mass);

	ImGui::End();
}
