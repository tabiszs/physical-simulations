#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"
#include "light.h"
#include "window.h"
#include "device.h"
#include "SpringSimulator.h"


using namespace std;

class Application
{
public:
	explicit Application();
	void MainLoop();
	void CleanUp();

protected:

private:
	void Menu();
	void Update();
	void Render();	
	void Swap();

	int width = 1200;
	int height = 800;
	const char* title = "Physics Simulations in Virtual Environment - Whirligig";
	float backgroundColor[4] = { 0.07f, 0.13f, 0.17f, 1.0f };
	glm::vec3 initTarget = glm::vec3(0.0f, 3.0f, 3.0f);
	float initDistance = 10.0f;
	float initNearPlane = 1.0f;
	float initFarPlane = 100.0f;
	float initMinDist = 0.01f;
	float initMaxDist = 50.0f;

	shared_ptr<Window> window;
	shared_ptr<Device> device;
	shared_ptr<Scene> scene;
	SpringSimulator ss = SpringSimulator();
	bool start = false;
	int speed = 1;
	double time = 0;
	double dt = 0;
	double step = 0.01;
};
