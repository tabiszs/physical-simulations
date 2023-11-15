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
