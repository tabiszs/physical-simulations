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

	// Variables Settings
	ImGui::SliderInt("Speed", &speed, 1, 10, "%d", ImGuiSliderFlags_AlwaysClamp);
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
	ImGui::SliderFloat("x0", &(ss.initial_deviation), -2, 2, "%f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat("v0", &(ss.v0), -2, 2, "%f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat("dt", &(ss.dt), 0.001, 0.5, "%f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat("m", &(ss.mass), 0, 5, "%f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat("c", &(ss.bounce), 0, 5, "%f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat("k", &(ss.k), 0, 1, "%f", ImGuiSliderFlags_AlwaysClamp);

	const char* items[] = { "Constant", "Sharp", "Sign", "Sin"};	
	ImGui::Combo("h(t)", &ss.h_fun_type, items, IM_ARRAYSIZE(items));
	SelectFunction(ss.h_fun_type, ss.h_params);
	ImGui::Combo("w(t)", &ss.w_fun_type, items, IM_ARRAYSIZE(items));
	SelectFunction(ss.w_fun_type, ss.w_params);
	ImGui::PopItemFlag();

	// Simulation Information
	ImGui::Text("Kinematics");
	ImGui::Text((std::string("x: " + to_string(ss.q)).c_str()));
	ImGui::Text((std::string("xt: " + to_string(ss.v)).c_str()));
	ImGui::Text((std::string("xtt: " + to_string(ss.a)).c_str()));
	ImGui::Text("Forces");
	ImGui::Text((std::string("f(t): " + to_string(ss.ft)).c_str()));
	ImGui::Text((std::string("g(t): " + to_string(ss.gt)).c_str()));
	ImGui::Text((std::string("h(t): " + to_string(ss.ht)).c_str()));
	ImGui::Text("Other");
	ImGui::Text((std::string("w(t): " + to_string(ss.wt)).c_str()));
	ImGui::End();

	// Plots
	ImGui::Begin("Plots");
	if (ImPlot::BeginPlot("Weight Plots", ImVec2(500, 250))) {
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ImPlot::SetupAxes("t", "y"/*, flags, flags*/);
		float t = ss.GetAcctualTime();
		float history = 10.0f;
		ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
		//ImPlot::SetupAxisLimits(ImAxis_X1, 0, 10);
		ImPlot::SetupAxisLimits(ImAxis_Y1, -2, 2);
		ImPlot::SetNextFillStyle({1,0,0,1});
		ImPlot::PlotLine("x(t)", ss.GetTimes(), ss.GetPositions(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::SetNextFillStyle({ 0,1,0,1 });
		ImPlot::PlotLine("xt(t)", ss.GetTimes(), ss.GetVelocities(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::SetNextFillStyle({ 0,0,1,1 });
		ImPlot::PlotLine("xtt(t)", ss.GetTimes(), ss.GetAccelerations(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::EndPlot();
	}
	if (ImPlot::BeginPlot("Forces Plots", ImVec2(100, 100))) {
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ImPlot::SetupAxes("t", "y"/*, flags, flags*/);
		float t = ss.GetAcctualTime();
		float history = 10.0f;
		ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
		//ImPlot::SetupAxisLimits(ImAxis_X1, 0, 10);
		ImPlot::SetupAxisLimits(ImAxis_Y1, -2, 2);
		ImPlot::SetNextFillStyle({ 1,0,0,1 });
		ImPlot::PlotLine("f(t)", ss.GetTimes(), ss.GetFts(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::SetNextFillStyle({ 0,1,0,1 });
		ImPlot::PlotLine("g(t)", ss.GetTimes(), ss.GetGts(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::SetNextFillStyle({ 0,0,1,1 });
		ImPlot::PlotLine("h(t)", ss.GetTimes(), ss.GetHts(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::SetNextFillStyle({ 1,1,0,1 });
		ImPlot::PlotLine("w(t)", ss.GetTimes(), ss.GetWts(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::EndPlot();
	}
	if (ImPlot::BeginPlot("State Plot", ImVec2(100,100))) {
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ImPlot::SetupAxes("x(t)", "xt(t)"/*, flags, flags*/);
		ImPlot::SetNextFillStyle({ 1,0,0,1 });
		ImPlot::PlotLine("state", ss.GetPositions(), ss.GetVelocities(), ss.GetSize(), 0, ss.GetOffset());
		ImPlot::EndPlot();
	}
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