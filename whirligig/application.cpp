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
	auto box = make_shared<Cube>();
	box->LoadMeshTo(device);
	scene->box = box;
	auto plane = make_shared<Plane>();
	plane->LoadMeshTo(device);
	scene->plane = plane;

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

	bool open = true;
	//ImGui::ShowDemoWindow(&open);

	//ImPlot::ShowDemoWindow();

	ImGui::Begin("Menu");
	// Controlls
	if (ImGui::Button("Start")) {
		start = true;
	}
	if (ImGui::Button("Stop")) {
		start = false;
	}
	if (ImGui::Button("Reset")) {
		start = false;
		ss.Reset();
	}
	ImGui::Separator();

	// Components
	bool cube, trajectory, diagonal, plane;
	ImGui::Text("Components");
	ImGui::Checkbox("Cube",&cube);
	ImGui::Checkbox("Cube Diagonal", &diagonal);
	ImGui::Checkbox("Cube Trajectory", &trajectory);
	ImGui::Checkbox("Ground Plane", &plane);
	ImGui::Separator();
	
	// Initial Conditions
	// wymiary
	// gestosc
	// wychylenie
	// predkosc katowa
	// dlugosc wyswietlanej trajektorii
	// wlaczenie wylaczenie grawitacji
	ImGui::End();
}

void Application::SelectFunction(const int& type, FunctionParameters& fun)
{
	switch (type)
	{
	case SpringSimulator::FunctionType::Constant:
		fun.aGui();
		break;
	case SpringSimulator::FunctionType::Sharp:
		fun.aGui();
		fun.tGui();
		break;
	case SpringSimulator::FunctionType::Sign:
		fun.aGui();
		fun.wGui();
		fun.phiGui();
		break;
	case SpringSimulator::FunctionType::Sin:
		fun.aGui();
		fun.wGui();
		fun.phiGui();
		break;
	}
	ImGui::Separator();
}

void Application::Update()
{
	device->CleanColor(backgroundColor);	
	if (start) {
		for (int i = 0; i < speed; ++i) {
			ss.Update();
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