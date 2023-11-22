#include "scene.h"
#include "mat.h"

void Scene::SetDevice(shared_ptr<Device> device)
{
	this->device = device;
}

void Scene::UpdateViewFrustrum(int width, int height)
{
	viewFrustrum->setViewportSize({ width, height });
}

void Scene::DrawOn(std::shared_ptr<Device> device)
{
}

void Scene::Update()
{
}

void Scene::Menu()
{
}

void Scene::UpdateProjViewMtx()
{
	m_viewProjMtx = viewFrustrum->getProjectionMatrix() * camera->ViewMatrix();	
}

void Scene::SetProjViewMtx(const std::shared_ptr<Shader> shader)
{
	shader->use();
	shader->setMatrix4F("projViewMtx", m_viewProjMtx);
}
