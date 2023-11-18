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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	cube->DrawModelOn(device);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	plane->DrawModelOn(device);
}

void Scene::Update()
{
	std::shared_ptr<Shader> shader;

	//if (box->need_update)
	{
		shader = cube->shader;
		shader->use();
		shader->set4Float("objectColor", cube->color);
		shader->setMatrix4F("modelMtx", cube->ModelMatrix());

		shader = cube->diagonal_shader;
		shader->use();
		shader->set4Float("objectColor", cube->diagonal_color);
		shader->setMatrix4F("modelMtx", cube->ModelMatrix());
		cube->need_update = false;
	}

	//if (plane->need_update)
	{
		shader = plane->shader;
		shader->use();
		shader->set4Float("objectColor", plane->color);
		shader->setMatrix4F("modelMtx", plane->ModelMatrix());
		plane->need_update = false;
	}

	//if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		shader = cube->shader;
		shader->use();
		cube->shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		shader = plane->shader;
		shader->use();
		plane->shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		shader = cube->diagonal_shader;
		shader->use();
		plane->shader->setMatrix4F("projViewMtx", m_viewProjMtx);

		camera->Updated();
		viewFrustrum->Updated();
	}	
}

void Scene::UpdateProjViewMtx()
{
	m_viewProjMtx = viewFrustrum->getProjectionMatrix() * camera->ViewMatrix();
	
}
