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
	if (showCube)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cube->DrawModelOn(device);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if(showDiagonal)
		cube->DrawDiagonalOn(device);
	if (showPlane)
		plane->DrawModelOn(device);
	if (showTrajectory)
		trajectory->DrawModelOn(device);
}

void Scene::Update()
{
	std::shared_ptr<Shader> shader;

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

		shader = cube->shader;
		shader->use();
		shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		shader = plane->shader;
		shader->use();
		shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		shader = cube->diagonal_shader;
		shader->use();
		shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		shader = trajectory->shader;
		shader->use();
		shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		camera->Updated();
		viewFrustrum->Updated();
	}	
}

void Scene::UpdateProjViewMtx()
{
	m_viewProjMtx = viewFrustrum->getProjectionMatrix() * camera->ViewMatrix();	
}
