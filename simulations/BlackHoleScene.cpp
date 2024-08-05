#include <glm/gtc/matrix_access.hpp>
#include "BlackHoleScene.h"


void BlackHoleScene::DrawOn(std::shared_ptr<Device> device)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glDepthMask(GL_FALSE);

	plane->DrawModelOn(device);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void BlackHoleScene::Update()
{
	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		const auto& viewport = viewFrustrum->viewportSize();
		plane->shader->use();
		plane->shader->setVec2("resolution", viewport.cx, viewport.cy);

		UpdateProjViewMtx();
		SetInvProjViewMtx(plane->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}

	if (distance_changed)
	{
		blackHolePosition.z = distance * distance_coefficient;
		plane->shader->use();
		plane->shader->set3Float("blackHolePosition", blackHolePosition);
		plane->shader->setFloat("blackHoleDistance", blackHolePosition.z);
	}

	if (mass_changed)
	{
		plane->shader->use();
		plane->shader->setFloat("mass", mass());
	}

	if (texture_changed)
	{
		if(show_debug_texture)
			device->BindCubemapTexture(debug_texture_id);
		else
			device->BindCubemapTexture(space_texture_id);
	}

	if (orbiting_camera_changed)
	{
		plane->shader->use();
		plane->shader->setBool("cameraOrbitAroundBlackHole", orbiting_camera);
	}
}

void BlackHoleScene::Menu()
{
	ImGui::SetNextWindowSize({ menu_width, menu_height }, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0.0f, 0.0f }, ImGuiCond_Always);
	ImGui::Begin("Black Hole", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	ImGui::PushItemWidth(100);
	distance_changed = ImGui::SliderFloat("Distance", &distance, min_distance, max_distance);
	ImGui::PopItemWidth();
	ImGui::SameLine(); ImGui::Text("x 1000 km");
	ImGui::PushItemWidth(100);
	mass_changed = ImGui::SliderInt("Mass", &mass_coefficient, min_mass_coefficient, max_mass_coefficient);
	ImGui::PopItemWidth();
	ImGui::SameLine(); ImGui::Text("x Being747");
	ImGui::Text("Being 747 mass: 200 ton");
	orbiting_camera_changed = ImGui::Checkbox("Orbiting Camera", &orbiting_camera);
	texture_changed = ImGui::Checkbox("Debug Texture", &show_debug_texture);
	ImGui::End();
}
