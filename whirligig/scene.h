#pragma once
#include "camera.h"
#include "light.h"
#include <memory>
#include "shader.h"
#include "device.h"
#include "ViewFrustrum.h"
#include "Box.h"
#include "Plane.h"


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
	void DrawOn(std::shared_ptr<Device> device);
	void Update();
	
	shared_ptr<Cube> box;
	shared_ptr<Plane> plane;
	shared_ptr<Light> light;
	shared_ptr<FpsCamera> camera;
	shared_ptr<ViewFrustrum> viewFrustrum;
private:
	void UpdateProjViewMtx();

	std::shared_ptr<Scene> getptr()
	{
		return shared_from_this();
	}
	shared_ptr<Device> device;

	glm::mat4 m_viewProjMtx;
	
};

