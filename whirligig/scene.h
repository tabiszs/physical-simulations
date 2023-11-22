#pragma once
#include "camera.h"
#include "light.h"
#include <memory>
#include "shader.h"
#include "device.h"
#include "ViewFrustrum.h"
#include "Cube.h"
#include "Plane.h"
#include "Trajectory.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <implot.h>

using namespace std;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
	{
		camera = c;
		light = l;
		viewFrustrum = vf;
		UpdateProjViewMtx();
	}
	void SetDevice(shared_ptr<Device> device);
	void UpdateViewFrustrum(int width, int height);
	virtual void DrawOn(std::shared_ptr<Device> device);
	virtual void Update();
	virtual void Menu();

	shared_ptr<Light> light;
	shared_ptr<FpsCamera> camera;
	shared_ptr<ViewFrustrum> viewFrustrum;

	bool start = false;
	int speed = 1;
	double time = 0;
	double dt = 0;
protected:
	void UpdateProjViewMtx();
	void SetProjViewMtx(const std::shared_ptr<Shader> shader);

	std::shared_ptr<Scene> getptr()
	{
		return shared_from_this();
	}
	shared_ptr<Device> device;

	glm::mat4 m_viewProjMtx;
	
};

