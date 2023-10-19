#pragma once
#include "camera.h"
#include "light.h"
#include <memory>
#include "shader.h"
#include "device.h"
#include "ViewFrustrum.h"


using namespace std;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
	{
		camera = c;
		light = l;
		viewFrustrum = vf;
	}
	void SetDevice(shared_ptr<Device> device);
	void UpdateViewFrustrum(int width, int height);
	void DrawOn(std::shared_ptr<Device> device);
	
	shared_ptr<Light> light;
	shared_ptr<FpsCamera> camera;
	shared_ptr<ViewFrustrum> viewFrustrum;
private:
	std::shared_ptr<Scene> getptr()
	{
		return shared_from_this();
	}
	shared_ptr<Device> device;
};

