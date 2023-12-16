#include "application.h"
#include "window.h"
#include "shader.h"
#include "ShaderHolder.h"
#include "utils.h"
#include "imGuiBuilder.h"
#include "Cube.h"
#include "ControlCube.h"
#include "Cursor.h"
#include "Arm.h"

Application::Application()
{
	device = shared_ptr<Device>(new Device());
	window = shared_ptr<Window>(new Window(width, height, title));
	auto& shaderHolder = ShaderHolder::Get();
	shaderHolder.Init();
	auto viewFrustum = make_shared<ViewFrustrum>(SIZE{ window->m_Width, window->m_Height }, M_PIDIV4, initNearPlane, initFarPlane);
	auto camera = shared_ptr<FpsCamera>(new FpsCamera(initTarget, initMinDist, initMaxDist, initDistance));
	auto light = shared_ptr<Light>(new Light((camera->Center())));

	whirligig_scene = std::make_shared<WhirligigScene>(camera, light, viewFrustum);
	whirligig_scene->SetDevice(device);
	window->ImportScene(whirligig_scene);
	auto cube = make_shared<WhirligigCube>();
	cube->LoadMeshTo(device);
	whirligig_scene->cube = cube;
	auto plane = make_shared<Plane>();
	plane->LoadMeshTo(device);
	whirligig_scene->plane = plane;
	auto trajectory = make_shared<Trajectory>();
	trajectory->LoadMeshTo(device);
	whirligig_scene->trajectory = trajectory;

	jelly_scene = std::make_shared<JellyScene>(camera, light, viewFrustum);
	jelly_scene->SetDevice(device);
	window->ImportScene(jelly_scene);
	auto bounding_cube = make_shared<Cube>(5.0f);
	bounding_cube->LoadMeshTo(device);
	jelly_scene->bounding_cube = bounding_cube;
	auto control_cube = make_shared<ControlCube>();
	control_cube->LoadMeshTo(device);
	jelly_scene->control_cube = control_cube;
	auto bezier_cube = make_shared<Jelly>();
	bezier_cube->LoadMeshTo(device);
	jelly_scene->jelly = bezier_cube;

	//interpolation_scene = std::make_shared<InterpolationScene>(camera, light, viewFrustum);
	////interpolation_scene->SetDevice(device);
	////interpolation_scene->UpdateViewFrustum(window->m_Width, window->m_Height);
	//window->ImportScene(interpolation_scene);
	//auto cursor = make_shared<Cursor>(ShaderHolder::Get().euler_cursorShader);
	//cursor->LoadMeshTo(device);
	//interpolation_scene->euler_cursor = cursor;
	//cursor = make_shared<Cursor>(ShaderHolder::Get().quat_cursorShader);
	//cursor->LoadMeshTo(device);
	//interpolation_scene->quat_cursor = cursor;
	//cursor = make_shared<Cursor>(ShaderHolder::Get().initial_cursorShader);
	//cursor->LoadMeshTo(device);
	//interpolation_scene->initial_cursor = cursor;
	//cursor = make_shared<Cursor>(ShaderHolder::Get().final_cursorShader);
	//cursor->LoadMeshTo(device);
	//cursor->position[2] = -0.5f;
	//cursor->position[2] = 2.0f;
	//interpolation_scene->final_cursor = cursor;

	//kinematic_chain_scene = std::make_shared<KinematicChainScene>(camera, light, viewFrustum, device);
	//window->ImportScene(kinematic_chain_scene);


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

	//whirligig_scene->Menu();
	jelly_scene->Menu();
	//interpolation_scene->Menu();
	//kinematic_chain_scene->Menu();
}

void Application::Update()
{
	device->CleanColor(backgroundColor);
	//whirligig_scene->Update();
	jelly_scene->Update();
	//interpolation_scene->Update();
	//kinematic_chain_scene->Update();
}

void Application::Render()
{
	//whirligig_scene->DrawOn(device);
	jelly_scene->DrawOn(device);
	//interpolation_scene->DrawOn(device);		
	//kinematic_chain_scene->DrawOn(device);
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