#include "application.h"
#include "window.h"
#include "shader.h"
#include "ShaderHolder.h"
#include "utils.h"
#include "imGuiBuilder.h"
#include <implot.h>

Application::Application()
{
	device = shared_ptr<Device>(new Device());
	window = shared_ptr<Window>(new Window(width, height, title));
	auto& shaderHolder = ShaderHolder::Get();
	shaderHolder.Init();
	auto viewFrustum = make_shared<ViewFrustrum>(SIZE{ window->m_Width, window->m_Height }, M_PIDIV4, initNearPlane, initFarPlane);
	auto camera = shared_ptr<FpsCamera>(new FpsCamera(initTarget, initMinDist, initMaxDist, initDistance));
	auto light = shared_ptr<Light>(new Light((camera->Center())));
	scene = unique_ptr<Scene>(new Scene(camera, light, viewFrustum));
	scene->SetDevice(device);
	window->ImportScene(scene);
	auto cube = make_shared<Cube>();
	cube->LoadMeshTo(device);
	scene->cube = cube;
	auto plane = make_shared<Plane>();
	plane->LoadMeshTo(device);
	scene->plane = plane;
	auto trajectory = make_shared<Trajectory>();
	trajectory->LoadMeshTo(device);
	scene->trajectory = trajectory;

	ImGuiBuilder::ImGuiBuilder(window->getWindow());
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(window->getWindow()))
	{		
		Menu();
		Update();
		Render();
		Swap();
	}
}

void Application::Menu()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Controls");
	if (ImGui::Button("Start")) {
		start = true;
		scene->cube->SetProperties();
		scene->trajectory->Clean();
		time = glfwGetTime();
		dt = 0;
	}
	if (ImGui::Button("Stop")) {
		start = false;
	}
	if (ImGui::Button("Reset")) {
		start = false;
		scene->cube->SetProperties();
		scene->trajectory->Clean();
	}
	ImGui::Separator();

	ImGui::Text("Components");
	ImGui::Checkbox("Cube",&scene->showCube);
	ImGui::Checkbox("Cube Diagonal", &scene->showDiagonal);
	ImGui::Checkbox("Cube Trajectory", &scene->showTrajectory);
	ImGui::Checkbox("Ground Plane", &scene->showPlane);
	ImGui::Separator();

	ImGui::Text("Initial Conditions");
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
	ImGui::SliderFloat("Cube size", &scene->cube->maxSize, 1.0f, 2.0f);
	ImGui::SliderFloat("Cube density", &scene->cube->density, 0.1f, 10.f);
	if (ImGui::SliderAngle("Cube inflection", &scene->cube->inflection, 0.0f, 90.0f))
		scene->cube->ChangedInflection();
	ImGui::SliderFloat("Cube angular momentum", &(scene->cube->w.y), 0, 10); // rad/s w kierunku y na scenie
	ImGui::SliderInt("Trajectory length", &(scene->trajectory->length), 1000, scene->trajectory->max_trajectory);
	ImGui::PopItemFlag();
	ImGui::Checkbox("Use gravitation", &scene->cube->use_gravitation);
	ImGui::SliderInt("Speed", &speed, 1, 100);
	ImGui::End();
}

void Application::Update()
{
	device->CleanColor(backgroundColor);

	if (start) {
		auto time2 = glfwGetTime();
		dt += time2 - time;
		time = time2;

		dt *= speed;
		auto step = scene->cube->dt;
		while (dt > step)
		{
			scene->cube->Update(step);
			glm::vec3 cone = scene->cube->GetConeCoordinates();
			scene->trajectory->push_back(cone);
			dt -= step;
		}	
	}	
	scene->Update();
}

void Application::Render()
{
	scene->DrawOn(device);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::Swap()
{
	glfwSwapBuffers(window->getWindow());
	glfwPollEvents();
	glfwSwapInterval(1);
}

void Application::CleanUp()
{
	ShaderHolder::Get().Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();

	glfwDestroyWindow(window->getWindow());
	glfwTerminate();
}