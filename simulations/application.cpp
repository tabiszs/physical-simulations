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

#define MODE 1
// 1 - Whirligig
// 2 - Jelly
// 3 - Quaternion Interpolation
// 4 - Kinematic Chain
// 5 - PUMA Robot
// 6 - Hodograph
// 7 - Black Hole

Application::Application()
{
	device = shared_ptr<Device>(new Device());
	window = shared_ptr<Window>(new Window(width, height, title));
	auto& shaderHolder = ShaderHolder::Get();
	shaderHolder.Init();
	auto viewFrustum = make_shared<ViewFrustrum>(SIZE{ window->m_Width, window->m_Height }, M_PIDIV4, initNearPlane, initFarPlane);
	auto camera = shared_ptr<FpsCamera>(new FpsCamera(initTarget, initMinDist, initMaxDist, initDistance));
	auto light = shared_ptr<Light>(new Light((camera->Center())));

#if MODE == 1
	auto whirligig_scene = std::make_shared<WhirligigScene>(camera, light, viewFrustum, device);
	window->ImportScene(whirligig_scene);
	selected_scene = whirligig_scene;
#elif MODE == 2
	auto jelly_scene = std::make_shared<JellyScene>(camera, light, viewFrustum, device);
	window->ImportScene(jelly_scene);
	selected_scene = jelly_scene;
#elif MODE == 3
	auto interpolation_scene = std::make_shared<InterpolationScene>(camera, light, viewFrustum, device);
	interpolation_scene->UpdateViewFrustum(window->m_Width, window->m_Height);
	window->ImportScene(interpolation_scene);
	selected_scene = interpolation_scene;
#elif MODE == 4
	auto kinematic_chain_scene = std::make_shared<KinematicChainScene>(camera, light, viewFrustum, device);
	window->ImportScene(kinematic_chain_scene);
	selected_scene = kinematic_chain_scene;
#elif MODE == 5
	auto puma_scene = std::make_shared<PumaScene>(camera, light, viewFrustum, device);
	puma_scene->UpdateViewFrustum(window->m_Width, window->m_Height);
	window->ImportScene(puma_scene);
	selected_scene = puma_scene;
#elif MODE == 6
	auto hodograph_scene = std::make_shared<HodographScene>(camera, light, viewFrustum, device);
	window->ImportScene(hodograph_scene);
	selected_scene = hodograph_scene;
#elif MODE == 7
	auto black_hole_scene = std::make_shared<BlackHoleScene>(camera, light, viewFrustum, device);
	window->ImportScene(black_hole_scene);
	selected_scene = black_hole_scene;
#else
	#error "Invalid MODE specified"
#endif

	ImGuiBuilder::ImGuiBuilder(window->getWindow());
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(window->getWindow()))
	{
		Update();
		Render();
		Menu();
		Swap();
	}
}

void Application::Menu()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	selected_scene->Menu();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::Update()
{
	device->CleanColor(backgroundColor);
	selected_scene->Update();
}

void Application::Render()
{
	selected_scene->DrawOn(device);	
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