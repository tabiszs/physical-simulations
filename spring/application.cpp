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
	ImGui::ShowDemoWindow(&open);

	ImPlot::ShowDemoWindow();
}

void Application::Update()
{
	device->CleanColor(backgroundColor);
	//draw
}

void Application::Render()
{
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